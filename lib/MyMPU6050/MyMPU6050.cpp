#include "MyMPU6050.h"


void MyMPU6050::initialize() {
  if (!this->_base.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1);
  }
  // Serial.println(F("MPU6050 Found!"));

  this->_base.setI2CBypass(true);

  this->_base.setAccelerometerRange(MPU6050_RANGE_8_G);
  this->_base.setGyroRange(MPU6050_RANGE_500_DEG);
  this->_base.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void MyMPU6050::calibrate() {
  Serial.println(F("Starting mpu6050 calibration..."));
  float AxOffset = 0, AyOffset = 0, AzOffset = 0;
  float GxOffset = 0, GyOffset = 0, GzOffset = 0;

  int samples = 250;
  
  for (int i = 0; i < samples; i++) {
    MPU6050Data data = this->readMPU6050FromSensor();
    AxOffset += data.Ax - 9.81f;
    AyOffset += data.Ay;
    AzOffset += data.Az;
    GxOffset += data.Gx;
    GyOffset += data.Gy;
    GzOffset += data.Gz;
    delay(20);
  }

  AxOffset /= samples;
  AyOffset /= samples;
  AzOffset /= samples;
  GxOffset /= samples;
  GyOffset /= samples;
  GzOffset /= samples;

  this->_calibrationData.Ax = -AxOffset;
  this->_calibrationData.Ay = -AyOffset;
  this->_calibrationData.Az = -AzOffset;
  this->_calibrationData.Gx = -GxOffset;
  this->_calibrationData.Gy = -GyOffset;
  this->_calibrationData.Gz = -GzOffset;



  Serial.println(F("Calibration complete!"));
  Serial.print(F("AxOffset: ")); Serial.println(AxOffset);
  Serial.print(F("AyOffset: ")); Serial.println(AyOffset);
  Serial.print(F("AzOffset: ")); Serial.println(AzOffset);
  Serial.print(F("GxOffset: ")); Serial.println(GxOffset);
  Serial.print(F("GyOffset: ")); Serial.println(GyOffset);
  Serial.print(F("GzOffset: ")); Serial.println(GzOffset);
}

MPU6050Data MyMPU6050::readMPU6050FromSensor() {
  MPU6050Data data;
  sensors_event_t a, g, temp;
  this->_base.getEvent(&a, &g, &temp);

  // RX Side for now
  // float AxOffset = -0.325f;
  // float AyOffset = -0.065f;
  // float AzOffset = +1.45f;

  // Serial.print("Raw Ax: "); Serial.println(a.acceleration.x);
  // Serial.print("Raw Ay: "); Serial.println(a.acceleration.y);
  // Serial.print("Raw Az: "); Serial.println(a.acceleration.z);

  data.Ax = a.acceleration.x + this->_calibrationData.Ax;
  data.Ay = a.acceleration.y + this->_calibrationData.Ay;
  data.Az = a.acceleration.z + this->_calibrationData.Az;

  // RX Side for now
  // float GxOffset = 0.049f;
  // float GyOffset = -0.0205f;
  // float GzOffset = 0.005f;

  data.Gx = g.gyro.x + this->_calibrationData.Gx;
  data.Gy = g.gyro.y + this->_calibrationData.Gy;
  data.Gz = g.gyro.z + this->_calibrationData.Gz;

  data.Temp = temp.temperature;

  return data;
}

float MyMPU6050::getAccelY() {
  sensors_event_t a, g, temp;
  this->_base.getEvent(&a, &g, &temp);
  float Ay = a.acceleration.y + this->_calibrationData.Ay;
  return Ay;
}

MPU6050Data MyMPU6050::readMPU6050FromBuffer(uint8_t* buffer) {
  MPU6050Data data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

void MyMPU6050::loadMPU6050ToBuffer(MPU6050Data& data, uint8_t* buffer) {
  memcpy(buffer+3, &data, sizeof(data));
}

void MyMPU6050::print() {
  MPU6050Data data = readMPU6050FromSensor();

  Serial.println("MPU6050 Data: ");
  Serial.print("Ax: "); Serial.println(data.Ax);
  Serial.print("Ay: "); Serial.println(data.Ay);  
  Serial.print("Az: "); Serial.println(data.Az);
  Serial.print("Gx: "); Serial.println(data.Gx);
  Serial.print("Gy: "); Serial.println(data.Gy);
  Serial.print("Gz: "); Serial.println(data.Gz);
  Serial.println("");
}