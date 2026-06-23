/**
 **************************************************
 *
 * @file        TFT-LCD-Breakout-2.4-With-Touch-SOLDERED.cpp
 * @brief       Functions for TFT touch screen.
 *
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Karlo Leksic for soldered.com
 ***************************************************/


#include "TFT-LCD-Breakout-2.4-With-Touch-SOLDERED.h"

/**
 * @brief Calibrate touch screen.
 *
 * @param int _xMin
 *        The raw value we get by the getXraw() function when touching the left edge of the display.
 *
 * @param int _xMax
 *        The raw value we get by the getXraw() function when touching the right edge of the display.
 *
 * @param int _yMin
 *        The raw value we get by the getYraw() function when touching the top edge of the display.
 *
 * @param int _yMax
 *        The raw value we get by the getYraw() function when touching the bottom edge of the display.
 */
void TFTTouch::calibrate(int _xMin, int _xMax, int _yMin, int _yMax)
{
    // The X and Y axis are swapped for our screen
    xMin = _yMin;
    xMax = _yMax;
    yMin = _xMin;
    yMax = _xMax;
}

/**
 *  @brief      Get X coordinate of the touched point on the screen.
 *
 *  @returns    int of the X coordinate of the touched point
 *
 */
int TFTTouch::getX()
{
    return map(ADS7846::getXraw(), xMin, xMax, 0, ILI9341_TFTWIDTH);
}

/**
 *  @brief      Get Y coordinate of the touched point on the screen.
 *
 *  @returns    int of the Y coordinate of the touched point
 *
 */
int TFTTouch::getY()
{
    return map(ADS7846::getYraw(), yMin, yMax, 0, ILI9341_TFTHEIGHT);
}
