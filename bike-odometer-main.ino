
#include <User_Setup.h>
#include <TFT_FastPin.h>
#include <TFT_ILI9341.h>
#include "bitmaps.h"
#include <Adafruit_BMP280.h>
#include <StopWatch.h>
#include <MapFloat.h>
#include <AnalogSmooth.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SPI.h>

//MCU: 1284P

// Pin definitions and wiring for TFT:
#define TFT_CS 4
#define TFT_DC 3
#define TFT_RST 2

/*Hardware MOSI and SCK pins for MEGA (can't be freely assigned):
  MOSI -> pin 51 (D5 for 1284)
  SCK -> pin 52. (D7 for 1284)
  LED & VCC -> 3.3v
  GND -> GND.
  SCK, MOSI, DC and CS pins must go through 5v -> 3.3v level shifter.
  MISO can be left disconnected if not using SD card.*/

// Create instance of TFT library:
TFT_ILI9341 tft = TFT_ILI9341();

#define backLight 13  //Pin to turn TFT backlight on and off. Must be a PWM pin.
int brightness = 0;   //Fade in/out brightness.
int bar = 0;          //Boot progress bar mockup.

bool displayOff = false;  //For turning display off/on when idle.
long unsigned displayOffTime;

//METERS STUFF:
//TO CHANGE MIN/MAX SPEED VALUES SEARCH "set min and max speed values" AND CHANGE THOSE LINES.

#define TFT_GREY 0x5AEB

float ltx = 0;                  // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120;  // Saved x & y coords
uint32_t updateTime = 0;        // time for next update

int old_analog = -999;   // Value last displayed
int old_digital = -999;  // Value last displayed

int d = 0;

Adafruit_BMP280 bmp;  // Initialise BMP280 library
long unsigned currentTempReadTime = 0;
long unsigned lastTempReadTime = 0;

long unsigned lastAltReadTime = 0;
long unsigned currentAltReadTime = 0;

volatile unsigned long lastTurn;
volatile unsigned long speedIncrease;
volatile float dSpeed;
volatile float newSpeed;
volatile float lastSpeedVal;
char dSpeedPrintout[5];
char oldSpeedPrintout[5];

volatile float totalDist;
volatile float lastTotalDistVal;
char totalDistPrintout[6];
char oldTotalDistPrintout[6];

volatile int maxSpeed;
volatile int lastMaxSpeedVal;
char maxSpeedPrintout[3];
char oldMaxSpeedPrintout[3];

volatile int avgSpeed;
volatile int lastAvgSpeedVal;
char avgSpeedPrintout[3];
char oldAvgSpeedPrintout[3];
volatile int turnCount = 0;
volatile int totalSpeed = 0;

volatile float todaysDist;
volatile float lastTodaysDistVal;
char todaysDistPrintout[5];
char oldTodaysDistPrintout[5];

volatile float newTemp;
volatile float oldTemp;
char tempPrintout[6];
char oldTempPrintout[6];

volatile float newAltitude;
volatile float oldAltitude;

volatile float ascentDist;
volatile float lastAscentDistVal;
char ascentPrintout[5];
char oldAscentPrintout[5];

volatile float descentDist;
volatile float lastDescentDistVal;
char descentPrintout[5];
char oldDescentPrintout[5];

float wheelCircum = 1.915;  //Wheel circumference (2πr).

//eeprom addresses & variables:
#define totalDistAddr 0
#define maxSpeedAddr totalDistAddr + sizeof(totalDist)
#define todaysDistAddr maxSpeedAddr + sizeof(totalDist)
#define ascentAddr todaysDistAddr + sizeof(totalDist)
#define descentAddr ascentAddr + sizeof(totalDist)
volatile boolean eepromState;
volatile boolean eepromFlagStart = false;
volatile long unsigned eepromFlagTime = 0;

//Button definitions & stopwatch variables:
#define button digitalRead(14)
byte buttonMode = 0;
bool buttonFlag = false;
long unsigned buttonTimer = 0;
long unsigned longPressTime = 800;
long unsigned modeTimeout = 0;
boolean buttonActive = false;
boolean longPressActive = false;

StopWatch sW;
byte mins = 0;
byte hours = 0;
bool resetOdometer = false;
char minsPrintout[3];
char hoursPrintout[3];

volatile float stopWatch;
volatile float lastStopWatchVal;
char stopWatchPrintout[6];
char oldStopWatchPrintout[6];

//Battery stuff:
int battPin = 0;                         //Analog pin to read battery voltage.
AnalogSmooth as400 = AnalogSmooth(400);  //Average value of 400 readings (library modified).
float battRead = 0;
char volPercPrintout[4];  //Char array for battery percentage.
long unsigned lastBattReadTime = -30000;
long unsigned currentBattReadTime = 0;
int voltagePerc = 0;
int lastVoltagePerc = 0;

void setup() {
  Serial.begin(115200);

  //Initialise TFT:
  tft.init();

  pinMode(10, INPUT_PULLUP);  //Hall sensor input. Must be interrupt pin.
  pinMode(button, INPUT);
  pinMode(15, OUTPUT);
  //digitalWrite(15, HIGH);  //Vcc for something.
  pinMode(backLight, OUTPUT);

  /*for (int i = 0 ; i < EEPROM.length() ; i++) //For resetting eeprom.
  {
      EEPROM.write(i, 0);
   }*/

  delay(100);

  //Boot sequence:
  tft.fillScreen(TFT_BLACK);
  tft.drawBitmap(10, 5, mando_riding, 240, 290, TFT_LIGHTGREY);
  tft.setTextColor(TFT_GREEN);
  tft.drawCentreString("v0.31b", 120, 300, 2);
  fadeIn();
  delay(3000);
  fadeOut();
  //Animation:
  tft.fillScreen(TFT_BLACK);
  analogWrite(backLight, 0);
  tft.drawBitmap(30, 35, stormtrooper, 190, 231, TFT_DARKGREY);
  fadeIn();
  fadeOut();
  tft.fillScreen(TFT_BLACK);
  tft.drawBitmap(30, 35, vader, 192, 215, TFT_RED);
  fadeIn();
  fadeOut();
  tft.fillScreen(TFT_BLACK);
  tft.drawBitmap(30, 35, mando, 180, 240, TFT_DARKGREY);
  fadeIn();
  fadeOut();
  tft.fillScreen(TFT_BLACK);
  tft.drawBitmap(30, 100, grogu, 197, 114, TFT_DARKGREEN);
  fadeIn();
  fadeOut();
  tft.fillScreen(TFT_BLACK);
  tft.drawBitmap(0, 50, system_loading, 240, 240, TFT_GREEN);
  fadeIn();

  for (bar = 20; bar <= 120; bar += 5) {
    tft.fillRoundRect(76, 230, bar, 15, 6, TFT_GREEN);
    delay(30);
  }
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(160, 210, 2);
  tft.print("100%");
  fadeOut();
  tft.fillScreen(TFT_BLACK);

  bmp.begin(0x76);                                  //0x76 address for this BMP280.
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */



  attachInterrupt(0, detectSensor, RISING);  //Hall sensor interrupt, INT0 is pin 16 of atmega1284 (D10).

  oldAltitude = bmp.readAltitude();  //Get starting altitude.

  //Read values from eeprom:
  totalDist = EEPROM.get(totalDistAddr, totalDist);
  //EEPROM.put(totalDistAddr, 154.00);             //In case it's accidentally erased.
  delay(100);
  maxSpeed = EEPROM.get(maxSpeedAddr, maxSpeed);
  //EEPROM.put(maxSpeedAddr, 40);
  delay(100);
  todaysDist = EEPROM.get(todaysDistAddr, todaysDist);
  //todaysDist = EEPROM.put(todaysDistAddr, 5.00);
  delay(100);
  ascentDist = EEPROM.get(ascentAddr, ascentDist);
  ///ascentDist = EEPROM.put(ascentAddr, 0.00);
  delay(100);
  descentDist = EEPROM.get(descentAddr, descentDist);
  //descentDist = EEPROM.put(descentAddr, 0.00);

  lastTurn = millis();

  //METER:
  analogMeter();          // Draw analogue meter
  updateTime = millis();  // Next update time

  //Draw grid:
  tft.drawFastHLine(0, 195, 240, TFT_GREY);    //Top horizontal.
  tft.drawFastVLine(150, 195, 125, TFT_GREY);  //Middle vertical.
  tft.drawFastHLine(0, 217, 150, TFT_GREY);    //Left horizontal 1.
  tft.drawFastHLine(0, 240, 150, TFT_GREY);    //Left horizontal 2.
  tft.drawFastHLine(0, 265, 150, TFT_GREY);    //Left horizontal 3.
  tft.drawFastHLine(150, 243, 90, TFT_GREY);   //Right horizontal 1.
  tft.drawFastHLine(150, 283, 90, TFT_GREY);   //Right horizontal 2.

  //Print value texts:
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("0.00", 120, 140, 7);
  tft.drawBitmap(158, 251, thermometer, 14, 23, TFT_RED);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setCursor(155, 200, 2);
  tft.print("Up:");
  tft.setCursor(155, 223, 2);
  tft.print("Dn:");
  tft.setTextColor(TFT_PINK, TFT_BLACK);
  tft.setCursor(2, 198, 2);
  tft.print("Max Speed:");
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.setCursor(2, 220, 2);
  tft.print("Avg Speed:");
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(2, 244, 2);
  tft.print("Today's Dist:");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(2, 270, 2);
  tft.print("Stopwatch:");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(30, 290, 4);
  tft.print("0:0:0.00");

  //If these values have been reset to 0, print them at boot:
  if (maxSpeed == 0) {
    printMaxSpeed();
  }
  if (avgSpeed == 0) {
    printAvgSpeed();
  }
  if (todaysDist == 0) {
    printTodaysDist();
  }
  if (ascentDist == 0) {
    printAscentDist();
  }
  if (descentDist == 0) {
    printDescentDist();
  }

  fadeIn();
}

void loop() {
  if (buttonMode == 5) {
    stopW();
  }

  //METER:
  if (updateTime <= millis()) {
    updateTime = millis() + 35;

    d += 4;
    if (d >= 360) d = 0;

    //unsigned long t = millis();

    plotNeedle(dSpeed, 10);  // It takes between 2 and 12ms to replot the needle with zero delay
    //Serial.println(millis()-t); // Print time taken for meter update
  }
  //END OF METER

  //Check if it's time to read BMP sensor
  currentTempReadTime = millis();
  currentAltReadTime = millis();
  readBmp();

  float analogSmooth400 = as400.analogReadSmooth(battPin);
  battRead = analogSmooth400;

  //Print averaged battery voltage every 30 seconds:
  currentBattReadTime = millis();
  readBattery();

  printValues();
  buttonModes();

  if ((millis() - lastTurn) > 3000 && (millis() - lastTurn < 3100))  //If there is no signal for 3 seconds.
  {
    dSpeed = 0;  //Speed is 0.

    eepromFlagStart == false;

    if (eepromState)  //If eepromState is true write values to memory.
    {
      EEPROM.put(totalDistAddr, totalDist);
      delay(100);
      EEPROM.put(maxSpeedAddr, maxSpeed);
      delay(100);
      EEPROM.put(todaysDistAddr, todaysDist);
      delay(100);
      EEPROM.put(ascentAddr, ascentDist);
      delay(100);
      EEPROM.put(descentAddr, descentDist);
      eepromState = 0;  //Flip eepromState to prevent rewriting.
    }
  }

  if ((millis() - displayOffTime) > 180000 && (millis() - displayOffTime < 180100))  //Turn off screen after 3 mins idle.
  {
    analogWrite(backLight, 0);
    displayOff = true;
  }

  if (dSpeed < 3.00)  //Halt stopwatch if speed goes below 3 km/h again.
  {
    sW.stop();
  }
}

void batteryIndicator() {
  float voltage = ((battRead * (3.3 / 1023.0)) * 1.47);  //Multiply by the ratio due to voltage divider.
  float perc = mapFloat(voltage, 3.0, 4.15, 0, 100);
  int voltagePerc = (int)perc;  //Cast float to int.
  voltagePerc = constrain(voltagePerc, 0, 100);

  String voltageVal = String(voltagePerc);
  voltageVal.toCharArray(volPercPrintout, 4);

  if (voltagePerc != lastVoltagePerc) {
    if (perc >= 75) {
      tft.drawBitmap(158, 290, battery, 14, 25, TFT_GREEN);
    }
    if (perc <= 74 && perc >= 50) {
      tft.drawBitmap(158, 290, battery, 14, 25, TFT_YELLOW);
    }
    if (perc <= 49 && perc >= 25) {
      tft.drawBitmap(158, 290, battery, 14, 25, TFT_ORANGE);
    }
    if (perc <= 24 && perc >= 0) {
      tft.drawBitmap(158, 290, battery, 14, 25, TFT_RED);
    }
    tft.fillRect(190, 292, 50, 20, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(190, 297, 2);
    tft.print(volPercPrintout);
    tft.println(" %");
  }
  lastVoltagePerc = voltagePerc;
}

void readBattery() {
  if (millis() - lastBattReadTime >= 30000) {
    batteryIndicator();
    lastBattReadTime = currentBattReadTime;
  }
}

void readBmp() {
  if (millis() - lastTempReadTime >= 5000) {
    readTemp();
    lastTempReadTime = currentTempReadTime;
  }

  if (dSpeed > 3.00 && millis() - lastAltReadTime >= 10000) {
    altitudeReading();
    lastAltReadTime = currentAltReadTime;
  }
}

void fadeIn() {
  for (brightness = 0; brightness <= 255; brightness += 5) {
    analogWrite(backLight, brightness);
    delay(10);
  }
}

void fadeOut() {
  for (brightness = 255; brightness >= 0; brightness -= 5) {
    analogWrite(backLight, brightness);
    delay(10);
  }
}