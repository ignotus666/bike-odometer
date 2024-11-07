void printValues() {
  //Current speed:
  String dSpeedVal = String(dSpeed);
  String oldSpeedVal = String(lastSpeedVal);
  dSpeedVal.toCharArray(dSpeedPrintout, 5);
  oldSpeedVal.toCharArray(oldSpeedPrintout, 5);
  if (lastSpeedVal != dSpeed) {
    if (displayOff == true) {
      analogWrite(backLight, 255);
    }
    tft.setTextColor(TFT_BLACK,TFT_BLACK);
    tft.drawCentreString(oldSpeedPrintout, 120, 140, 7);
    if (dSpeed <= 15) {
      tft.setTextColor(TFT_GREEN,TFT_BLACK);
    }
    if (dSpeed > 15 && dSpeed <= 30) {
      tft.setTextColor(TFT_YELLOW,TFT_BLACK);
    }
    if (dSpeed > 30 && dSpeed <= 45) {
      tft.setTextColor(TFT_ORANGE,TFT_BLACK);
    }
    if (dSpeed > 45) {
      tft.setTextColor(TFT_RED,TFT_BLACK);
    }
    tft.drawCentreString(dSpeedPrintout, 120, 140, 7);
  }
  lastSpeedVal = dSpeed;

  //Total distance:
  String totalDistVal = String(totalDist);
  String oldTotalDistVal = String(lastTotalDistVal);
  totalDistVal.toCharArray(totalDistPrintout, 6);
  oldTotalDistVal.toCharArray(oldTotalDistPrintout, 6);
  tft.setCursor(120, 70, 4);
  if (lastTotalDistVal != totalDist) {
    tft.setTextColor(TFT_BLACK);
    tft.drawCentreString(oldTotalDistPrintout, 120, 70, 4);
    tft.setTextColor(TFT_CYAN);
    tft.drawCentreString(totalDistPrintout, 120, 70, 4);
  }
  lastTotalDistVal = totalDist;

  //Max speed:
  if (lastMaxSpeedVal != maxSpeed) {
    printMaxSpeed();
  }
  lastMaxSpeedVal = maxSpeed;

  //Average speed:
  if (lastAvgSpeedVal != avgSpeed) {
    printAvgSpeed();
  }
  lastAvgSpeedVal = avgSpeed;

  //Today's distance:
  if (lastTodaysDistVal != todaysDist) {
    printTodaysDist();
  }
  lastTodaysDistVal = todaysDist;

  //Ascent distance:
  if (lastAscentDistVal != ascentDist) {
    printAscentDist();
  }
  lastAscentDistVal = ascentDist;

  //Descent distance:
  if (lastDescentDistVal != descentDist) {
    printDescentDist();
  }
  lastDescentDistVal = descentDist;
}

void printMaxSpeed() {
  String maxSpeedVal = String(maxSpeed);
  String oldMaxSpeedVal = String(lastMaxSpeedVal);
  maxSpeedVal.toCharArray(maxSpeedPrintout, 3);
  oldMaxSpeedVal.toCharArray(oldMaxSpeedPrintout, 3);
  tft.setTextColor(TFT_BLACK);                           //Print old value in black to erase it.
  tft.setCursor(85, 198, 2);
  tft.print(oldMaxSpeedPrintout);
  tft.println(" km/h");
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(85, 198, 2);
  tft.print(maxSpeedPrintout);
  tft.println(" km/h");
}

void printAvgSpeed() {
  String avgSpeedVal = String(avgSpeed);
  String oldAvgSpeedVal = String(lastAvgSpeedVal);
  avgSpeedVal.toCharArray(avgSpeedPrintout, 3);
  oldAvgSpeedVal.toCharArray(oldAvgSpeedPrintout, 3);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(85, 220, 2);
  tft.print(oldAvgSpeedPrintout);
  tft.println(" km/h");
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(85, 220, 2);
  tft.print(avgSpeedPrintout);
  tft.println(" km/h");
}

void printTodaysDist() {
  String todaysDistVal = String(todaysDist);
  String oldTodaysDistVal = String(lastTodaysDistVal);
  todaysDistVal.toCharArray(todaysDistPrintout, 5);
  oldTodaysDistVal.toCharArray(oldTodaysDistPrintout, 5);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(85, 244, 2);
  tft.print(oldTodaysDistPrintout);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(85, 244, 2);
  tft.print(todaysDistPrintout);
  tft.println(" km");
}

void printAscentDist() {
  String ascentVal = String(ascentDist);
  String oldAscentVal = String(lastAscentDistVal);
  ascentVal.toCharArray(ascentPrintout, 5);
  oldAscentVal.toCharArray(oldAscentPrintout, 5);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(175, 200, 2);
  tft.print(oldAscentPrintout);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(175, 200, 2);
  tft.print(ascentPrintout);
  tft.println(" m");
}

void printDescentDist() {
  String descentVal = String(descentDist);
  String oldDescentVal = String(lastDescentDistVal);
  descentVal.toCharArray(descentPrintout, 5);
  oldDescentVal.toCharArray(oldDescentPrintout, 5);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(175, 223, 2);
  tft.print(oldDescentPrintout);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(175, 223, 2);
  tft.print(descentPrintout);
  tft.println(" m");
}