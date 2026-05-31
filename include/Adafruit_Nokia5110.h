#ifndef ADAFRUIT_Nokia5110_H
#define ADAFRUIT_Nokia5110_H

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "UTILS.h"

// #define DC A2
// #define CE 8
// #define RST A3

#define DC A3
#define CE A2
#define RST 8


// Blue Nokia5110 doesnt work with 4Mhz, changing Adafruit Library Hardware SPI Speed is required.
void InitScreen(Adafruit_PCD8544& display);

void DisplayButtons(Adafruit_PCD8544& display, ButtonsData& data);
void DisplayGyro(Adafruit_PCD8544& display, MPU6050Data& data, Header& header);
void DisplayAccel(Adafruit_PCD8544& display, MPU6050Data& data, Header& header);
void DisplayBaro(Adafruit_PCD8544& display, BMP180Data& data, Header& header);
void DisplayMag(Adafruit_PCD8544& display, QMCL588LData& data, Header& header);


#endif // ADAFRUIT_Nokia5110_H