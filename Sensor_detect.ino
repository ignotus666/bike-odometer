
void detectSensor()
{
  if (millis() - lastTurn > 120)                                           //Noise filter. Two readings in less than 120 ms (~120 km/h) will be ignored.
  {
    dSpeed = wheelCircum / ((float)(millis() - lastTurn) / 1000) * 3.6;    //Calculate speed in km/h.
    totalSpeed += dSpeed;                                                  //Every turn there's a reading of speed. Add each reading to previous total.
    turnCount += 1;                                                        //Divide aggregate speed by number of turns (readings) to get average.

    if (millis() - speedIncrease >= 2000)                                  //If speed increases by 10 km/h or more within 2 s, discard new maxSpeed (inserting/removing sensor cable while on causes spikes). 
    {
      if (dSpeed - newSpeed <= 10)
      {
        if (dSpeed > maxSpeed)
        {
          maxSpeed = dSpeed;
        }
      }
      speedIncrease = millis();
      newSpeed = dSpeed;
    }

    lastTurn = millis();                                                   //Remember time of last revolution.
    displayOffTime = millis();
    totalDist = totalDist + wheelCircum / 1000;                            //Calculate total distance.
    todaysDist = todaysDist + wheelCircum / 1000;                          //Calculate today's distance.
    avgSpeed = totalSpeed / turnCount;                                     //Calculate average speed.

    if (displayOff == true)                                                //Turn backlight on if it was off.
    {
      analogWrite(backLight, 255);
      displayOff = false;
    }

    if (eepromFlagStart == false)                                           //Prevent eeprom flag from flipping until sure bike is rolling.
    {
      eepromFlagTime = millis();
      eepromFlagStart = true;
    }

    if (millis() - eepromFlagTime > 2000)
    {
      eepromState = 1;
    }

    if (buttonMode == 5)                                                    //If stopwatch is active, start it when sensor goes high.
    {
      sW.start();
    }
  }
}
