#include "MyDisplay.h"

void MyDisplay::initialize() {
    this->_base.begin();
    this->_base.setContrast(55);
    this->_base.clearDisplay();
    this->_base.setTextSize(1);
    this->_base.setTextColor(BLACK);
}

void MyDisplay::displayButtons(ButtonsData& data) {
  this->_base.clearDisplay();
  this->_base.setCursor(0,0);
  this->_base.println(F("-TX BUTTONS-"));
  
  this->_base.print(F("X: ")); this->_base.println(data.joystickX);
  this->_base.print(F("Y: ")); this->_base.println(data.joystickY);

  this->_base.print(F("Btns: "));
  if(data.ButtonA == HIGH) this->_base.print(F("A"));
  if(data.ButtonB == HIGH) this->_base.print(F("B"));
  if(data.ButtonC == HIGH) this->_base.print(F("C"));
  if(data.ButtonD == HIGH) this->_base.print(F("D"));
  if(data.ButtonE == HIGH) this->_base.print(F("E"));
  if(data.ButtonF == HIGH) this->_base.print(F("F"));

  this->_base.display();
}

void MyDisplay::displayGyro(MPU6050Data& data, bool& TX_screen) {
  this->_base.clearDisplay();
  this->_base.setCursor(0,0);

  this->_base.print((TX_screen) ? F("TX") : F("RX")); 
  this->_base.println(F(" GYRO (deg/s)"));

  this->_base.print(F("X: ")); this->_base.println(data.Gx, 5);
  this->_base.print(F("Y: ")); this->_base.println(data.Gy, 5);
  this->_base.print(F("Z: ")); this->_base.println(data.Gz, 5);

  this->_base.display();
}

void MyDisplay::displayAccel(MPU6050Data& data, bool& TX_screen) {
  this->_base.clearDisplay();
  this->_base.setCursor(0,0);

  this->_base.print((TX_screen) ? F("TX") : F("RX")); 
  this->_base.println(F(" ACCEL (m/s^2)"));

  this->_base.print(F("X: ")); this->_base.println(data.Ax, 5);
  this->_base.print(F("Y: ")); this->_base.println(data.Ay, 5);
  this->_base.print(F("Z: ")); this->_base.println(data.Az, 5);

  this->_base.display();
}

void MyDisplay::displayBaro(BMP180Data& data, bool& TX_screen) {
  this->_base.clearDisplay();
  this->_base.setCursor(0,0);

  this->_base.print((TX_screen) ? F("-- TX") : F("-- RX")); 
  this->_base.println(F(" BARO --"));
  this->_base.println(F("C, hPa, m"));

  this->_base.print(F("Temp: ")); this->_base.println(data.Temp, 2);
  this->_base.print(F("Pres: ")); this->_base.println(data.Pressure);

  this->_base.print(F("Sea: ")); this->_base.println(data.SeaLevelPressure);
  this->_base.print(F("Alt: ")); this->_base.println(data.Altitude, 2);

  this->_base.display();
}

void MyDisplay::displayMag(CompassData& data, bool& TX_screen) {
  this->_base.clearDisplay();
  this->_base.setCursor(0,0);

  this->_base.print((TX_screen) ? F("TX") : F("RX")); 
  this->_base.println(F(" COMPASS"));

  this->_base.print(F("X: ")); this->_base.println(data.x);
  this->_base.print(F("Y: ")); this->_base.println(data.y);
  this->_base.print(F("Z: ")); this->_base.println(data.z);
  this->_base.print(F("A:")); this->_base.println(data.a);
  this->_base.print(F("Dir: ")); this->_base.write(data.Direction, 3);

  this->_base.display();
}

void MyDisplay::displaySpeed(SpeedData& data, bool& TX_screen) {
  this->_base.clearDisplay();
  this->_base.setCursor(0,0);

  this->_base.print((TX_screen) ? F("TX") : F("RX")); 
  this->_base.println(F(" Speeed"));

  this->_base.print(F("ACC: ")); this->_base.println(data.current_acceleration, 5);
  this->_base.print(F("VEL: ")); this->_base.println(data.current_velocity, 5);
  this->_base.print(F("DIST: ")); this->_base.println(data.current_distance, 5);

  this->_base.display();

}