/*--------------------------------------------------------------------------
  GUGGENHAT: a Bluefruit LE-enabled wearable NeoPixel marquee.

  Requires:
  - Arduino Micro or Leonardo microcontroller board.  An Arduino Uno will
    NOT work -- Bluetooth plus the large NeoPixel array requires the extra
    512 bytes available on the Micro/Leonardo boards.
  - Adafruit Bluefruit LE nRF8001 breakout: www.adafruit.com/products/1697
  - 4 Meters 60 NeoPixel LED strip: www.adafruit.com/product/1461
  - 3xAA alkaline cells, 4xAA NiMH or a beefy (e.g. 1200 mAh) LiPo battery.
  - Late-model Android or iOS phone or tablet running nRF UART or
    Bluefruit LE Connect app.
  - BLE_UART, NeoPixel, NeoMatrix and GFX libraries for Arduino.

  Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.
  MIT license.  All text above must be included in any redistribution.
  --------------------------------------------------------------------------*/

//#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

// NEOPIXEL STUFF ----------------------------------------------------------

// 4 meters of NeoPixel strip is coiled around a top hat; the result is
// not a perfect grid.  My large-ish 61cm circumference hat accommodates
// 37 pixels around...a 240 pixel reel isn't quite enough for 7 rows all
// around, so there's 7 rows at the front, 6 at the back; a smaller hat
// will fare better.
#define NEO_PIN     6  // Arduino pin to NeoPixel data input
#define NEO_WIDTH   75 // Hat circumference in pixels
#define NEO_HEIGHT  7  // Number of pixel rows (round up if not equal)
#define NEO_OFFSET  74 // (((NEO_WIDTH * NEO_HEIGHT) - 240) / 2)

// Pixel strip must be coiled counterclockwise, top to bottom, due to
// custom remap function (not a regular grid).
Adafruit_NeoMatrix matrix(NEO_WIDTH, NEO_HEIGHT, NEO_PIN,
  NEO_MATRIX_TOP  + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB         + NEO_KHZ800);

char          msg[24]          = "Tinkersmiths Makerspace"; // BLE 20 char limit + NUL
uint8_t       msgLen           = NULL;              // Empty message
int           msgX             = matrix.width();    // Start off right edge
unsigned long prevFrameTime    = 0L;                // For animation timing
int           matrixFPS        = 15;                // Scrolling speed
int           matrixColor      = matrix.Color(255,0,0); // RGB
int           matrixBrightness = 50;                // Batteries have limited sauce (0 to 255)

// STATUS LED STUFF --------------------------------------------------------

// The Arduino's onboard LED indicates BTLE status.  Fast flash = waiting
// for connection, slow flash = connected, off = disconnected.
#define LED 13                   // Onboard LED (not NeoPixel) pin
int           LEDperiod   = 0;   // Time (milliseconds) between LED toggles
boolean       LEDstate    = LOW; // LED flashing state HIGH/LOW
unsigned long prevLEDtime = 0L;  // For LED timing


// UTILITY FUNCTIONS -------------------------------------------------------

// Because the NeoPixel strip is coiled and not a uniform grid, a special
// remapping function is used for the NeoMatrix library.  Given an X and Y
// grid position, this returns the corresponding strip pixel number.
// Any off-strip pixels are automatically clipped by the NeoPixel library.
uint16_t remapXY(uint16_t x, uint16_t y) {
  return y * NEO_WIDTH - x + NEO_OFFSET;
}

// Given hexadecimal character [0-9,a-f], return decimal value (0 if invalid)
uint8_t unhex(char c) {
  return ((c >= '0') && (c <= '9')) ?      c - '0' :
         ((c >= 'a') && (c <= 'f')) ? 10 + c - 'a' :
         ((c >= 'A') && (c <= 'F')) ? 10 + c - 'A' : 0;
}


void setup() {
  matrix.begin();
  matrix.setRemapFunction(remapXY);
  matrix.setTextWrap(false);   // Allow scrolling off left
  matrix.setBrightness(matrixBrightness);    // Batteries have limited sauce


  pinMode(LED, OUTPUT);
  LEDstate    = HIGH;
  digitalWrite(LED, LEDstate);
  
  //set it flashing on 1sec interval
  LEDperiod = 1000;
  
}

void loop() {
  unsigned long t = millis(); // Current elapsed time, milliseconds.
  // millis() comparisons are used rather than delay() so that animation
  // speed is consistent regardless of message length & other factors.

  matrixColor = matrix.Color(0,128,128);// RGB
  matrix.setTextColor(matrixColor); // Red by default

  if(LEDperiod && ((t - prevLEDtime) >= LEDperiod)) { // Handle LED flash
    prevLEDtime = t;
    LEDstate    = !LEDstate;
    digitalWrite(LED, LEDstate);
  }

  if((t - prevFrameTime) >= (1000L / matrixFPS)) { // Handle scrolling
    matrix.fillScreen(0);
    matrix.setCursor(msgX, 0);
    matrix.print(msg);
    msgLen = sizeof(msg);
    if(--msgX < (msgLen * -6)) msgX = matrix.width(); // We must repeat!
    matrix.show();
    prevFrameTime = t;
  }
  
}

