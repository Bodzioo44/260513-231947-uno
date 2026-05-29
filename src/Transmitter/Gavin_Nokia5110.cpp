#include "Gavin_Nokia5110.h"

void InitScreen(NOKIA5110_TEXT& display) {
  delay(100);
  display.LCDInit(inverse, contrast, bias);
  display.LCDClear(0x00); 

  Serial.println("Nokia 5110 LCD Initialized");
}

void DisplayPilotButtons(NOKIA5110_TEXT& display, uint8_t* buffer) {
    display.LCDFont(LCDFont_Default);
    display.LCDClear(0x00);
    display.LCDgotoXY(0, 0);
    display.LCDString("-BUTTONS-");
    display.LCDgotoXY(0, 1);
    display.LCDString("HELLO WORLD");

    Serial.print("DISPLAYING BUTTONS");
}