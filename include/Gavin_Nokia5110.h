#ifndef GAVIN_NOKIA5110_H
#define GAVIN_NOKIA5110_H

#include <NOKIA5110_TEXT.h>



#define DC A3
#define CE A2
#define RST 8

#define inverse  false // set to true to invert display pixel color
#define contrast 0xBF // default is 0xBF set in LCDinit, Try 0xB1 <-> 0xBF if your display is too dark
#define bias 0x12 // LCD bias mode 1:48: Try 0x12 or 0x13 or 0x14


void InitScreen(NOKIA5110_TEXT& display);

void DisplayPilotGyro(NOKIA5110_TEXT& display, uint8_t* buffer);
void DisplayPilotAccel(NOKIA5110_TEXT& display, uint8_t* buffer);
void DisplayPilotButtons(NOKIA5110_TEXT& display, uint8_t* buffer);
void DisplayPilotBarometer(NOKIA5110_TEXT& display, uint8_t* buffer);





#endif // GAVIN_NOKIA5110_H