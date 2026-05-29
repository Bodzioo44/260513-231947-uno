#include "MPU6050.h"

void initializeQMC5883L(QMC5883LCompass& compass) {
  compass.init();
  compass.setCalibrationOffsets(105.00, 130.00, -677.00);
  compass.setCalibrationScales(1.01, 0.86, 1.18);
  Serial.println(F("QMC5883L Found!"));
}

void initializeMPU6050(Adafruit_MPU6050& mpu) {
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1);
  }
  Serial.println(F("MPU6050 Found!"));

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void initializeBMP180(Adafruit_BMP085& bmp) {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP180 sensor, check wiring!"));
    while (1);
  }
  Serial.println(F("BMP180 Found!"));
}

MPU6050Data ReadMPU6050(Adafruit_MPU6050& mpu) {
  MPU6050Data data;
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  data.Ax = a.acceleration.x;
  data.Ay = a.acceleration.y;
  data.Az = a.acceleration.z;

  data.Gx = g.gyro.x;
  data.Gy = g.gyro.y;
  data.Gz = g.gyro.z;

  data.Temp = temp.temperature;

  return data;
}

void LoadMPU6050(MPU6050Data& data, uint8_t* buffer) {
  memcpy(buffer, &data, sizeof(data));
}

// void LoadMPU6050(Adafruit_MPU6050& mpu, uint8_t* buffer) {
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   *buffer++ = FloatToUint8(a.acceleration.x);
//   *buffer++ = FloatToUint8(a.acceleration.y);
//   *buffer++ = FloatToUint8(a.acceleration.z);

//   *buffer++ = FloatToUint8(g.gyro.x);
//   *buffer++ = FloatToUint8(g.gyro.y);
//   *buffer++ = FloatToUint8(g.gyro.z);
// }

// void LoadBMP180(Adafruit_BMP085& bmp, uint8_t* buffer) {
//   float temperature = bmp.readTemperature();
//   uint16_t pressure = bmp.readPressure()/1000;

//   *buffer++ = FloatToUint8(temperature);
//   *buffer++ = (pressure >> 8) & 0xFF;
//   *buffer++ = pressure & 0xFF;
// }

// void LoadQMC5883L(QMC5883LCompass& compass, uint8_t* buffer) {

// 	compass.read();

//   *buffer++ = compass.getX() >> 8;
//   *buffer++ = compass.getX() & 0xFF;

//   *buffer++ = compass.getY() >> 8;
//   *buffer++ = compass.getY() & 0xFF;

//   *buffer++ = compass.getZ() >> 8;
//   *buffer++ = compass.getZ() & 0xFF;

// 	compass.getDirection((char*) buffer, compass.getAzimuth());

// }





void printBMP180(Adafruit_BMP085& bmp) {
  Serial.println();
  Serial.println(F("BMP180 ------------"));
  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(F(" Pa"));

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print(F("Altitude = "));
  Serial.print(bmp.readAltitude());
  Serial.println(F(" meters"));

  Serial.print(F("Pressure at sealevel (calculated) = "));
  Serial.print(bmp.readSealevelPressure());
  Serial.println(F(" Pa"));
  Serial.println(F("BMP180 ------------"));
  Serial.println();
}


void printMPU6050(Adafruit_MPU6050& mpu) {

  Serial.println();
  Serial.println("MPU6050 ------------");

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("MPU6050 ------------");
  Serial.println();
}

void printQMC5883L(QMC5883LCompass& compass) {

  Serial.println();
  Serial.println("QMC5883L ------------");

	int x, y, z, a;
	char myArray[3];
	
	compass.read();
  
	x = compass.getX();
	y = compass.getY();
	z = compass.getZ();
	
	a = compass.getAzimuth();

	compass.getDirection(myArray, a);
  
	Serial.print("X: ");
	Serial.print(x);

	Serial.print(" Y: ");
	Serial.print(y);

	Serial.print(" Z: ");
	Serial.print(z);

	Serial.print(" Azimuth: ");
	Serial.print(a);

	Serial.print(" Direction: ");
	Serial.print(myArray[0]);
	Serial.print(myArray[1]);
	Serial.println(myArray[2]);

  Serial.println("QMC5883L ------------");
  Serial.println();
}