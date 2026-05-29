#include "Adafruit_Nokia5110.h"

void InitScreen(Adafruit_PCD8544& display) {
  display.begin();
  display.setContrast(55);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
}

void DisplayPilotButtons(Adafruit_PCD8544& display, uint8_t* buffer) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("-BUTTONS-"));

  display.print(F("X: ")); display.println(((buffer[7] << 8) | buffer[6]));
  display.print(F("Y: ")); display.println(((buffer[9] << 8) | buffer[8]));
  
  //   display.print(F("Link: ")); 
  //   display.println(success ? F("OK") : F("LOST"));
  
  display.print(F("Btns: "));
  if(buffer[0] == HIGH) display.print(F("A"));
  if(buffer[1] == HIGH) display.print(F("B"));
  if(buffer[2] == HIGH) display.print(F("C"));
  if(buffer[3] == HIGH) display.print(F("D"));
  if(buffer[4] == HIGH) display.print(F("E"));
  if(buffer[5] == HIGH) display.print(F("F"));

  display.display();
}

void DisplayPilotAccel(Adafruit_PCD8544& display, uint8_t* buffer) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("ACCEL (m/s^2)"));

  float temp = (int8_t)buffer[9]/10.0f;
  display.print(F("X: ")); display.println(temp, 1);
  temp = (int8_t)buffer[10]/10.0f;
  display.print(F("Y: ")); display.println(temp, 1); 
  temp = (int8_t)buffer[11]/10.0f;
  display.print(F("Z: ")); display.println(temp, 1);

  display.display();
}
  
void DisplayPilotGyro(Adafruit_PCD8544& display, uint8_t* buffer) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("GYRO (deg/s)"));

  float temp = (int8_t)buffer[12]/10.0f;
  display.print(F("X: ")); display.println(temp, 1);
  temp = (int8_t)buffer[13]/10.0f;
  display.print(F("Y: ")); display.println(temp, 1);
  temp = (int8_t)buffer[14]/10.0f;
  display.print(F("Z: ")); display.println(temp, 1);

  display.display();
}

void DisplayPilotBarometer(Adafruit_PCD8544& display, uint8_t* buffer) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("BAROMETER"));

  float temperature = buffer[15]/10.0f;
  uint16_t pressure = (buffer[17] << 8) | buffer[16];

  display.print(F("Temp: ")); display.print(temperature, 1); display.println(F(" C"));
  display.print(F("Pres: ")); display.print(pressure); display.println(F(" kPa"));

  display.display();
}
