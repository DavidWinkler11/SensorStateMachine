/**
 **************************************************
 *
 * @file        TFT-LCD-Breakout-2.4-With-Touch-SOLDERED.h
 * @brief       Header file for TFTDisplay.
 *
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Karlo Leksic for soldered.com
 ***************************************************/

#ifndef __TFT_SOLDERED_H__
#define __TFT_SOLDERED_H__

#include "Arduino.h"
#include "libs/ADS7846/ADS7846.h"
#include "libs/Adafruit_ILI9341/Adafruit_ILI9341.h"

class TFTDisplay : public Adafruit_ILI9341
{
  public:
    TFTDisplay(int cs, int dc, int rst) : Adafruit_ILI9341(cs, dc, rst)
    {
    }
};

class TFTTouch : public ADS7846
{
  public:
    TFTTouch(uint8_t cs_pin) : ADS7846(cs_pin)
    {
    }

    void calibrate(int _xMin, int _xMax, int _yMin, int _yMax);
    int getX();
    int getY();

  private:
    int xMin, xMax, yMin, yMax;
};

#endif
