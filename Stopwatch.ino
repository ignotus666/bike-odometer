void stopW()
{
  float stopWatchNum = ((float)sW.elapsed() / 1000);

  String stopWatchVal = String(stopWatchNum);
  String oldStopWatchVal = String(lastStopWatchVal);

  stopWatchVal.toCharArray(stopWatchPrintout, 6);
  oldStopWatchVal.toCharArray(oldStopWatchPrintout, 6);

  String minsVal = String(mins);
  minsVal.toCharArray(minsPrintout, 3);
  
  String hoursVal = String(hours);
  hoursVal.toCharArray(hoursPrintout, 3);

  if (lastStopWatchVal != stopWatch)
  {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(30, 290, 4);
    tft.print(hoursPrintout);
    tft.print(":");
    tft.print(minsPrintout);
    tft.print(":");
    tft.print(stopWatchPrintout);
  }
  lastStopWatchVal = stopWatchNum;

  //Serial.println(sW);

  if (sW.elapsed() >= 60000)    //Hack to display minutes.
  {
    tft.fillRect(30, 290, 120, 50, TFT_BLACK);
    mins++;
    sW.stop();
    sW.reset();
    sW.start();
  }

  if (mins >= 60)              //Hack to display hours.
  {
    tft.fillRect(30, 290, 120, 50, TFT_BLACK);
    hours++;
    sW.stop();
    sW.reset();
    sW.start();
  }
}
