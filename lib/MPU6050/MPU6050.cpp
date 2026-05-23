#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <QMC5883LCompass.h>

void initializeQMC5883L(QMC5883LCompass& compass) {
  compass.init();
  compass.setCalibrationOffsets(105.00, 130.00, -677.00);
  compass.setCalibrationScales(1.01, 0.86, 1.18);
  Serial.println(F("QMC5883L Found!"));
}

void LoadQMC5883L(QMC5883LCompass& compass, uint8_t* buffer) {

	compass.read();

  *buffer++ = compass.getX() >> 8;
  *buffer++ = compass.getX() & 0xFF;

  *buffer++ = compass.getY() >> 8;
  *buffer++ = compass.getY() & 0xFF;

  *buffer++ = compass.getZ() >> 8;
  *buffer++ = compass.getZ() & 0xFF;
	compass.getDirection((char*) buffer, compass.getAzimuth());

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

void printMPU6050(Adafruit_MPU6050& mpu, uint8_t* buffer) {

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  *buffer++ = 


  /* Print out the values */
  Serial.print(F("Acceleration X: "));
  Serial.print(a.acceleration.x);
  Serial.print(F(", Y: "));
  Serial.print(a.acceleration.y);
  Serial.print(F(", Z: "));
  Serial.print(a.acceleration.z);
  Serial.println(F(" m/s^2"));

  Serial.print(F("Rotation X: "));
  Serial.print(g.gyro.x);
  Serial.print(F(", Y: "));
  Serial.print(g.gyro.y);
  Serial.print(F(", Z: "));
  Serial.print(g.gyro.z);
  Serial.println(F(" rad/s"));

  Serial.print(F("Temperature: "));
  Serial.print(temp.temperature);
  Serial.println(F(" degC"));

  Serial.println(F("MPU6050 ------------"));
  Serial.println();
}

void initializeBMP180(Adafruit_BMP085& bmp) {
  // Start BMP180 initialization
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP180 sensor, check wiring!"));
    while (1)
      ;  // Halt if sensor not found
  }
  Serial.println(F("BMP180 Found!"));
}

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