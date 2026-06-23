/**
 **************************************************
 *
 * @file        TouchScreenRaw.ino
 * @brief       Read the raw data from the touch screen
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagrams below
 *              -Change DC and CS pins if needed
 *              -Open Serial monitor at 115200 baud
 *              -Touch the display with your finger!
 *
 *              TFT LCD breakout 2.4" with touch: solde.red/333211
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 * @authors     Robert @ soldered.com
 *              Based on Adafruit's TouchScreenRaw Example
 ***************************************************/

// Include needed libraries
#include "Adafruit_GFX.h"
#include "SPI.h"
#include "TFT-LCD-Breakout-2.4-With-Touch-SOLDERED.h"

// Define pins for DC, CS, and Reset
#define TFT_DC    9
#define TFT_CSL   6
#define TFT_RST -1 // If not used, use -1

// Reccomended pins for DC, CS, and Reset for Connect/ConnectPlus
// #define TFT_DC    5
// #define TFT_CSL   4
// #define TFT_RST 16 // If not used, use -1

/**
 * Connecting diagram:
 *
 * TFT LCD breakout 2.4"        Dasduino Core / Connect / ConnectPlus
 * VCC------------------------->VCC
 * GND------------------------->GND
 * DC-------------------------->TFT_DC, set by user
 * BL-------------------------->VCC
 * CLK------------------------->D13/IO14/IO14
 * D0-------------------------->D11/IO12/IO12 // This isn't nescessary to connect
 * DI-------------------------->D12/IO13/IO13
 * CSL------------------------->TFT_CSL, set by user
 * CST------------------------->D10/IO15/IO15
 * RST------------------------->TFT_RST, set by user, optional
 */

// Create the display object
TFTDisplay tft(TFT_CSL, TFT_DC, TFT_RST);

// Create the touchscreen object
// Use the same pin as CST
TFTTouch ts(10);

void setup()
{
    // Init serial communication
    Serial.begin(115200);

    // Init Touch-Controller
    Serial.println("Initializing...");
    ts.begin();

    // Init display
    tft.begin();

    // Print message to know default orientation
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(5);
    tft.println("Hello!");
    tft.setTextSize(2);
    tft.println("Touch the screen and check the Serial output at 112500 baud!");

    // If you want to get pressed pixel value when touching the screen instead 
    // of weird numbers, you have to calibrate the touchscreen.
    // For calibration run this example and touch the edges of the screen. 
    // Remember the numbers for both axis printed on the serial monitor and 
    // call the function calibrate() with these parameters. For example: 
    // ts.calibrate(540, 1000, 34, 460);
    // Parameters are: 
    // X value when touching the left edge of the display
    // X value when touching the right edge of the display
    // Y value when touching the top edge of the display
    // Y value when touching the bottom edge of the display
}


void loop()
{
    // Read data from the touchscreen
    ts.service();

    // Get raw values of the x, and y coordinates, and pressure (z coordinate)
    uint16_t x = ts.getXraw();
    uint16_t y = ts.getYraw();
    uint16_t z = ts.getPressure();

    // Check if the touchscreen is touched
    // NOTE: The screen must be pressed a bit more than on a smartphone to detect touch
    if (z)
    {
        // Print the raw values on the Serial Monitor
        Serial.print("X: ");
        Serial.print(x, DEC);
        Serial.print(" Y: ");
        Serial.print(y, DEC);
        Serial.print(" Pressure: ");
        Serial.println(z, DEC);
    }

    // Wait a bit
    delay(100);
}