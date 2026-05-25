#ifndef ADAFRUIT_Nokia5110_H
#define ADAFRUIT_Nokia5110_H

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


void InitScreen(Adafruit_PCD8544& display);

void DisplayPilotGyro(Adafruit_PCD8544& display, uint8_t* buffer);
void DisplayPilotAccel(Adafruit_PCD8544& display, uint8_t* buffer);
void DisplayPilotButtons(Adafruit_PCD8544& display, uint8_t* buffer);
void DisplayPilotBarometer(Adafruit_PCD8544& display, uint8_t* buffer);


#endif // ADAFRUIT_Nokia5110_H