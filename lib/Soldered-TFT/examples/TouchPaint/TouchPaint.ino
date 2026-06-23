/**
 **************************************************
 *
 * @file        GraphicsText.ino
 * @brief       This example will create a simple painting program on the touch screen.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagrams below
 *              -Change DC and CS pins if needed
 *              -Paint on the display with your finger!
 *
 * @note        You may want to calibrate your touchscreen first, check TouchScreenRaw.ino example
 *
 *              TFT LCD breakout 2.4" with touch: solde.red/333211
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 * @authors     Robert @ soldered.com
 *              Based on Adafruit's TouchPaint Example
 ***************************************************/

// Include needed libraries
#include "Adafruit_GFX.h"
#include "SPI.h"
#include "TFT-LCD-Breakout-2.4-With-Touch-SOLDERED.h"

// Define pins for DC, CS, and Reset
#define TFT_DC  9
#define TFT_CSL 6
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

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE   40
#define PENRADIUS 3
int oldcolor, currentcolor;

// Struct for the points in the coordinate system
struct point
{
    int x;
    int y;
} p;

void setup(void)
{
    // Init serial communication
    Serial.begin(115200);
    Serial.println(F("Touch Paint!"));

    // Init tft display and touchscreen
    tft.begin();
    ts.begin();
    Serial.println("Touchscreen started");

    // Calibrate your touch like in the example TouchScreenRaw
    ts.calibrate(540, 1000, 34, 460);

    // Put black background
    tft.fillScreen(ILI9341_BLACK);

    // Make the color selection boxes
    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
    tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
    tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
    tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
    tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
    tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);

    // Select the current color 'red'
    tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    currentcolor = ILI9341_RED;
}


void loop()
{
    // Read data from the touchscreen
    ts.service();

    // Check if the touchscreen is touched
    // NOTE: The screen must be pressed a bit more than on a smartphone to detect touch
    if (!ts.getPressure())
    {
        return;
    }

    // Get pressed coordinates
    p.x = ts.getX();
    p.y = ts.getY();

    // Print the coordinates of the pressed point on the screen
    Serial.print("X = ");
    Serial.print(p.x);
    Serial.print("\tY = ");
    Serial.println(p.y);


    if (p.y < BOXSIZE)
    {
        oldcolor = currentcolor;

        if (p.x < BOXSIZE)
        {
            currentcolor = ILI9341_RED;
            tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }
        else if (p.x < BOXSIZE * 2)
        {
            currentcolor = ILI9341_YELLOW;
            tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }
        else if (p.x < BOXSIZE * 3)
        {
            currentcolor = ILI9341_GREEN;
            tft.drawRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }
        else if (p.x < BOXSIZE * 4)
        {
            currentcolor = ILI9341_CYAN;
            tft.drawRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }
        else if (p.x < BOXSIZE * 5)
        {
            currentcolor = ILI9341_BLUE;
            tft.drawRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }
        else if (p.x < BOXSIZE * 6)
        {
            currentcolor = ILI9341_MAGENTA;
            tft.drawRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9341_WHITE);
        }

        if (oldcolor != currentcolor)
        {
            if (oldcolor == ILI9341_RED)
                tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
            if (oldcolor == ILI9341_YELLOW)
                tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
            if (oldcolor == ILI9341_GREEN)
                tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
            if (oldcolor == ILI9341_CYAN)
                tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
            if (oldcolor == ILI9341_BLUE)
                tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
            if (oldcolor == ILI9341_MAGENTA)
                tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, ILI9341_MAGENTA);
        }
    }
    if (((p.y - PENRADIUS) > BOXSIZE) && ((p.y + PENRADIUS) < tft.height()))
    {
        tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
    }
}