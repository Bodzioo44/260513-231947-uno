#include "MyMPU6050.h"


void MyMPU6050::initialize() {
  if (!this->_base.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1);
  }
  Serial.println(F("MPU6050 Found!"));

  this->_base.setI2CBypass(true);

  this->_base.setAccelerometerRange(MPU6050_RANGE_8_G);
  this->_base.setGyroRange(MPU6050_RANGE_500_DEG);
  this->_base.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

MPU6050Data MyMPU6050::readMPU6050FromSensor() {
  MPU6050Data data;
  sensors_event_t a, g, temp;
  this->_base.getEvent(&a, &g, &temp);

  // RX Side for now
  float AxOffset = -0.325f;
  float AyOffset = -0.065f;
  float AzOffset = +1.45f;

  data.Ax = a.acceleration.x + AxOffset;
  data.Ay = a.acceleration.y + AyOffset;
  data.Az = a.acceleration.z + AzOffset;

  // RX Side for now
  float GxOffset = 0.049f;
  float GyOffset = -0.0205f;
  float GzOffset = 0.005f;

  data.Gx = g.gyro.x + GxOffset;
  data.Gy = g.gyro.y + GyOffset;
  data.Gz = g.gyro.z + GzOffset;

  data.Temp = temp.temperature;

  return data;
}

MPU6050Data MyMPU6050::readMPU6050FromBuffer(uint8_t* buffer) {
  MPU6050Data data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

void MyMPU6050::loadMPU6050ToBuffer(MPU6050Data& data, uint8_t* buffer) {
  memcpy(buffer+3, &data, sizeof(data));
}