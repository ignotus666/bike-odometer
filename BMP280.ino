void readTemp() {
  newTemp = bmp.readTemperature();
  String tempVal = String(newTemp);
  String oldTempVal = String(oldTemp);
  tempVal.toCharArray(tempPrintout, 6);
  oldTempVal.toCharArray(oldTempPrintout, 6);
  if (oldTemp != newTemp) {
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(180, 256, 2);
    tft.print(oldTempPrintout);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(180, 256, 2);
    tft.print(tempPrintout);
    //Serial.print(bmp.readTemperature());
    tft.println("C");
  }
  oldTemp = newTemp;
}

void altitudeReading() {
  newAltitude = bmp.readAltitude();

  if (newAltitude > oldAltitude) {
    ascentDist = ascentDist + (newAltitude - oldAltitude);
  }

  if (newAltitude < oldAltitude) {
    descentDist = descentDist + (oldAltitude - newAltitude);
  }
  oldAltitude = newAltitude;
}
