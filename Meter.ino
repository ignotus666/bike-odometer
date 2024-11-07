void analogMeter()
{
  // Meter outline
  //tft.fillRect(0, 0, 239, 156, TFT_GREY);
  //tft.fillRect(5, 3, 230, 149, TFT_WHITE);

  tft.setTextColor(TFT_WHITE);  // Text colour

  // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  for (int i = -50; i < 51; i += 5) {
    // Long scale tick length
    int tl = 15;

    // Coodinates of tick to draw
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (100 + tl) + 120;
    uint16_t y0 = sy * (100 + tl) + 150;
    uint16_t x1 = sx * 100 + 120;
    uint16_t y1 = sy * 100 + 150;

    // Coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (100 + tl) + 120;
    int y2 = sy2 * (100 + tl) + 150;
    int x3 = sx2 * 100 + 120;
    int y3 = sy2 * 100 + 150;

    // Green zone limits
    if (i >= -50 && i < -25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_GREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_GREEN);
    }

    // Yellow zone limits
    if (i >= -25 && i < 0) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_YELLOW);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_YELLOW);
    }

    // Orange zone limits
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_ORANGE);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_ORANGE);
    }

    // Red zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, TFT_RED);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, TFT_RED);
    }

    // Short scale tick length
    if (i % 25 != 0) tl = 8;

    // Recalculate coords in case tick length changed
    x0 = sx * (100 + tl) + 120;
    y0 = sy * (100 + tl) + 150;
    x1 = sx * 100 + 120;
    y1 = sy * 100 + 150;

    // Draw tick
    tft.drawLine(x0, y0, x1, y1, TFT_WHITE);

    // Check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (100 + tl + 10) + 120;
      y0 = sy * (100 + tl + 10) + 150;
      switch (i / 25) {
        case -2: tft.drawCentreString("0", x0, y0 - 18, 4); break;
        case -1: tft.drawCentreString("15", x0, y0 - 18, 2); break;
        case 0: tft.drawCentreString("30", x0, y0 - 17, 4); break;
        case 1: tft.drawCentreString("45", x0, y0 - 18, 2); break;
        case 2: tft.drawCentreString("60", x0, y0 - 19, 4); break;
      }
    }

    // Now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * 100 + 120;
    y0 = sy * 100 + 150;
    // Draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, TFT_BLACK);
  }

  tft.drawString("km/h", 5 + 230 - 40, 119 - 20, 2); // Units at bottom right
  
  String totalDistVal = String(totalDist);
  totalDistVal.toCharArray(totalDistPrintout, 6);  
  tft.drawCentreString(totalDistPrintout, 120, 70, 4); // Comment out to avoid font 4
  tft.setTextColor(TFT_CYAN);
  tft.drawCentreString("km", 120, 95, 2);
  //tft.drawRect(5, 3, 230, 149, TFT_BLACK); // Draw bezel line

  plotNeedle(0, 0); // Put meter needle at 0
}

// #########################################################################
// Update needle position
// This function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)
// #########################################################################
void plotNeedle(int value, byte ms_delay)
{
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //char buf[8]; dtostrf(value, 4, 0, buf);
  //tft.drawRightString(buf, 40, 149 - 20, 2);

  if (value < 0) value = 0;   // Limit value to emulate needle end stops
  if (value > 60) value = 60; //Change to set min and max speed values in meter

  // Move the needle util new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;

    if (ms_delay == 0) old_analog = value; // Update immediately if delay is 0

    float sdeg = map(old_analog, 0, 60, -140, -40); // Map value to angle; set min and max speed values
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // Erase old needle image
    tft.drawLine(120 + 20 * ltx - 1, 150 - 20, osx - 1, osy, TFT_BLACK);
    tft.drawLine(120 + 20 * ltx, 150 - 20, osx, osy, TFT_BLACK);
    tft.drawLine(120 + 20 * ltx + 1, 150 - 20, osx + 1, osy, TFT_BLACK);

    // Re-plot text under needle
    tft.setTextColor(TFT_CYAN);
    tft.setTextSize(1);
    tft.drawCentreString(totalDistPrintout, 120, 70, 4); // Comment out to avoid font 4
    tft.drawCentreString("km", 120, 95, 2);

    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 98 + 120;
    osy = sy * 98 + 150;

    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(120 + 20 * ltx - 1, 150 - 20, osx - 1, osy, TFT_MAGENTA);
    tft.drawLine(120 + 20 * ltx, 150 - 20, osx, osy, TFT_WHITE);
    tft.drawLine(120 + 20 * ltx + 1, 150 - 20, osx + 1, osy, TFT_MAGENTA);

    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    // Wait before next update
    delay(ms_delay);
  }
}
