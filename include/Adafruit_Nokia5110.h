#ifndef ADAFRUIT_Nokia5110_H
#define ADAFRUIT_Nokia5110_H

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// #define DC A2
// #define CE 8
// #define RST A3

#define DC A3
#define CE A2
#define RST 8


// Blue Nokia5110 doesnt work with 4Mhz, changing Adafruit Library Hardware SPI Speed is required.
void InitScreen(Adafruit_PCD8544& display);

void DisplayPilotGyro(Adafruit_PCD8544& display, uint8_t* buffer);
void DisplayPilotAccel(Adafruit_PCD8544& display, uint8_t* buffer);
void DisplayPilotButtons(Adafruit_PCD8544& display, uint8_t* buffer);
void DisplayPilotBarometer(Adafruit_PCD8544& display, uint8_t* buffer);


#endif // ADAFRUIT_Nokia5110_H