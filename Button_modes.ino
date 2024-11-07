void buttonModes()
{
  if (button == LOW)
  {
    if (buttonActive == false)
    {
      buttonActive = true;
      buttonTimer = millis();
    }

    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) //Long press.
    {
      modeTimeout = millis();                                                   //Restart menu exit timeout.

      if (buttonMode == 0 && resetOdometer == false)
      {
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.drawCentreString(totalDistPrintout, 120, 70, 4);
        resetOdometer = true;
      }

      else if (buttonMode == 0 && resetOdometer == true)
      {
        tft.fillRect(78, 70, 90, 26, TFT_BLACK);
        totalDist = 0;
        EEPROM.put(totalDistAddr, totalDist);
        resetOdometer = false;
      }

      if (buttonMode == 1)
      {
        maxSpeed = 0;                                                          //Reset top speed ever.
        EEPROM.put(maxSpeedAddr, maxSpeed);
      }

      else if (buttonMode == 2)
      {
        avgSpeed = 0;                                                        //Reset average speed.
      }

      else if (buttonMode == 3)
      {
        todaysDist = 0;                                                      //Reset today's distance.
        EEPROM.put(todaysDistAddr, todaysDist);
      }

      else if (buttonMode == 4)
      {
        ascentDist = 0;                                                      //Reset up/down distance.
        descentDist = 0;
        EEPROM.put(ascentAddr, ascentDist);
        EEPROM.put(descentAddr, descentDist);
      }

      else if (buttonMode == 5)
      {
        sW.reset();                                                             //Reset stopwatch to 0.
        mins = 0;
        float stopWatchNum = ((float)sW.elapsed() / 1000);
        String stopWatchVal = String(stopWatch);
        stopWatchVal.toCharArray(stopWatchPrintout, 5);

        String minsVal = String(mins);
        minsVal.toCharArray(minsPrintout, 3);

        String hoursVal = String(hours);
        hoursVal.toCharArray(hoursPrintout, 3);

        tft.fillRect(30, 290, 120, 50, TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(30, 290, 4);
        tft.print(hoursPrintout);
        tft.print(":");
        tft.print(minsPrintout);
        tft.print(":");
        tft.print(stopWatchPrintout);
      }

      longPressActive = true;
    }
  }
  else                                                                          //Otherwise short press.
  {
    if (buttonActive == true)
    {
      if (longPressActive == true)
      {
        longPressActive = false;
      }

      else
      {
        if (displayOff == true)
        {
          analogWrite(backLight, 255);
          displayOff = false;
        }

        else if (displayOff == false)
        {
          buttonMode++;
        }

        displayOffTime = millis();

        if (buttonMode > 5)
        {
          buttonMode = 0;
        }

        buttonFlag = true;

        if (resetOdometer == true)
        {
          resetOdometer = false;
          tft.setTextColor(TFT_CYAN, TFT_BLACK);
          tft.drawCentreString(totalDistPrintout, 120, 70, 4);
          buttonMode = 0;
        }
      }
      buttonActive = false;
    }
  }

  //End of 'if button pressed'.

  if (displayOff == false)
  {
    if (buttonMode == 1 && buttonFlag == true)
    {
      //Highlight max speed
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(2, 198, 2);
      tft.print("Max Speed");
      modeTimeout = millis();
      buttonFlag = false;
    }

    else if (buttonMode == 2 && buttonFlag == true)
    {
      //Highlight average speed
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.setCursor(2, 198, 2);
      tft.print("Max Speed");
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(2, 220, 2);
      tft.print("Avg Speed");
      modeTimeout = millis();
      buttonFlag = false;
    }

    else if (buttonMode == 3 && buttonFlag == true)
    {
      //Highlight today's distance
      tft.setTextColor(TFT_BLUE, TFT_BLACK);
      tft.setCursor(2, 220, 2);
      tft.print("Avg Speed");
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(2, 244, 2);
      tft.print("Today's Dist");
      modeTimeout = millis();
      buttonFlag = false;
    }

    else if (buttonMode == 4 && buttonFlag == true)
    {
      //Highlight altitude changes
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(2, 244, 2);
      tft.print("Today's Dist:");
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(155, 200, 2);
      tft.print("Up");
      tft.setCursor(155, 223, 2);
      tft.print("Dn");
      modeTimeout = millis();
      buttonFlag = false;
    }

    else if (buttonMode == 5 && buttonFlag == true)
    {
      //Highlight sw.
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setCursor(155, 200, 2);
      tft.print("Up");
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setCursor(155, 223, 2);
      tft.print("Dn");
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(2, 270, 2);
      tft.print("Stopwatch");
      buttonFlag = false;
    }

    else if (buttonMode == 0 && buttonFlag == true)
    {
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.setCursor(2, 270, 2);
      tft.print("Stopwatch");
      buttonFlag = false;
    }
  }

  //Return to main screen after timeout:
  if (millis() - modeTimeout >= 4000 && millis() - modeTimeout <= 4050)
  {
    if (buttonMode == 1)
    {
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.setCursor(2, 198, 2);
      tft.print("Max Speed");
      buttonMode = 0;
    }

    if (buttonMode == 2)
    {
      tft.setTextColor(TFT_BLUE, TFT_BLACK);
      tft.setCursor(2, 220, 2);
      tft.print("Avg Speed");
      buttonMode = 0;
    }

    if (buttonMode == 3)
    {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(2, 244, 2);
      tft.print("Today's Dist:");
      buttonMode = 0;
    }

    if (buttonMode == 4)
    {
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setCursor(155, 200, 2);
      tft.print("Up");
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.setCursor(155, 223, 2);
      tft.print("Dn");
      buttonMode = 0;
    }

    if (resetOdometer == true)
    {
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.drawCentreString(totalDistPrintout, 120, 70, 4);
      buttonMode = 0;
      resetOdometer = false;
    }
  }
}
