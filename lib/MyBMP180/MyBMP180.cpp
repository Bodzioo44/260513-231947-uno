#include "MyBMP180.h"




void MyBMP180::initialize() {
  if (!this->_base.begin()) {
    Serial.println(F("Failed to find BMP180 chip"));
    while (1);
  }
  Serial.println(F("BMP180 Found!"));
}


BMP180Data MyBMP180::readBMP180FromSensor() {
  BMP180Data data;
  data.Temp = this->_base.readRawTemperature()/1000.0f;
  data.Pressure = this->_base.readPressure();
  data.Altitude = this->_base.readAltitude();
  data.SeaLevelPressure = this->_base.readSealevelPressure();

  return data;
}


BMP180Data MyBMP180::readBMP180FromBuffer(uint8_t* buffer) {
  BMP180Data data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

void MyBMP180::loadBMP180ToBuffer(BMP180Data& data, uint8_t* buffer) {
  memcpy(buffer+3, &data, sizeof(data));
}

