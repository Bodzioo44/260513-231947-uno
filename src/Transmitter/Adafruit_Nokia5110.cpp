#include "Adafruit_Nokia5110.h"

void InitScreen(Adafruit_PCD8544& display) {
  display.begin();
  display.setContrast(55);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
}


void DisplayButtons(Adafruit_PCD8544& display, ButtonsData& data) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("-TX BUTTONS-"));
  
  display.print(F("X: ")); display.println(data.joystickX);
  display.print(F("Y: ")); display.println(data.joystickY);

  display.print(F("Btns: "));
  if(data.ButtonA == HIGH) display.print(F("A"));
  if(data.ButtonB == HIGH) display.print(F("B"));
  if(data.ButtonC == HIGH) display.print(F("C"));
  if(data.ButtonD == HIGH) display.print(F("D"));
  if(data.ButtonE == HIGH) display.print(F("E"));
  if(data.ButtonF == HIGH) display.print(F("F"));

  display.display();
}

void DisplayGyro(Adafruit_PCD8544& display, MPU6050Data& data, Header& header) {
  display.clearDisplay();
  display.setCursor(0,0);

  // display.print((header.DataType = DATA_TYPE::MPU6050_DATA_TX) ? F("TX") : F("RX")); 
  display.println(F(" GYRO (deg/s)"));

  display.print(F("X: ")); display.println(data.Gx, 1);
  display.print(F("Y: ")); display.println(data.Gy, 1);
  display.print(F("Z: ")); display.println(data.Gz, 1);

  display.display();
}

void DisplayAccel(Adafruit_PCD8544& display, MPU6050Data& data, Header& header) {
  display.clearDisplay();
  display.setCursor(0,0);

  // display.print((header.DataType = DATA_TYPE::MPU6050_DATA_TX) ? F("TX") : F("RX")); 
  display.println(F(" ACCEL (m/s^2)"));

  display.print(F("X: ")); display.println(data.Ax, 1);
  display.print(F("Y: ")); display.println(data.Ay, 1);
  display.print(F("Z: ")); display.println(data.Az, 1);

  display.display();
}

void DisplayBaro(Adafruit_PCD8544& display, BMP180Data& data, Header& header) {
  display.clearDisplay();
  display.setCursor(0,0);

  // display.print((header.DataType = DATA_TYPE::BMP180_DATA_TX) ? F("TX") : F("RX")); 
  display.println(F(" BAROMETER"));

  display.print(F("Temp: ")); display.print(data.Temp, 2); display.println(F(" C"));
  display.print(F("Pres: ")); display.print(data.Pressure); display.println(F(" kPa"));
  display.print(F("SeaPres: ")); display.print(data.SeaLevelPressure); display.println(F(" kPa"));
  display.print(F("Alt: ")); display.print(data.Altitude, 2); display.println(F(" m"));

  display.display();
}

void DisplayMag(Adafruit_PCD8544& display, QMCL588LData& data, Header& header) {
  display.clearDisplay();
  display.setCursor(0,0);

  // display.print((header.DataType = DATA_TYPE::QMCL588L_DATA_TX) ? F("TX") : F("RX")); 
  display.println(F(" MAGNETOMETR"));

  display.print(F("X: ")); display.println(data.x); // display.println(F(" C"));
  display.print(F("Y: ")); display.println(data.y); // display.println(F(" kPa"));
  display.print(F("Z: ")); display.println(data.z); // display.println(F(" kPa"));
  display.print(F("Az: ")); display.println(data.a); // display.println(F(" m"));
  display.print(F("Dir: ")); display.println(data.Direction); // display.println(F(" m"));

  display.display();
}