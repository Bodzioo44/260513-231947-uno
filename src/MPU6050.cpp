#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <QMC5883LCompass.h>

void initializeQMC5883L(QMC5883LCompass& compass) {

  
  compass.init();

  // compass.setCalibrationOffsets(-336.00, -179.00, 47.00);
  // compass.setCalibrationScales(1.05, 0.94, 1.02);
  compass.setCalibrationOffsets(105.00, 130.00, -677.00);
  compass.setCalibrationScales(1.01, 0.86, 1.18);

  Serial.println(F("QMC5883L Found!"));

}

void printQMC5883L(QMC5883LCompass& compass) {

  Serial.println(F("QMC5883L ------------"));

	int x, y, z, a;
	char myArray[3];
	
	compass.read();
  
	x = compass.getX();
	y = compass.getY();
	z = compass.getZ();
	
	a = compass.getAzimuth();

	compass.getDirection(myArray, a);
  
	Serial.print(F("X: "));
	Serial.print(x);

	Serial.print(F(" Y: "));
	Serial.print(y);

	Serial.print(F(" Z: "));
	Serial.print(z);

	Serial.print(F(" Azimuth: "));
	Serial.print(a);

	Serial.print(F(" Direction: "));
	Serial.print(myArray[0]);
	Serial.print(myArray[1]);
	Serial.println(myArray[2]);

  Serial.println(F("QMC5883L ------------"));
  Serial.println();
}


void initializeMPU6050(Adafruit_MPU6050& mpu) {
  // Check if the MPU6050 sensor is detected
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1)
      ;  // Halt if sensor not found
  }
  Serial.println(F("MPU6050 Found!"));

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

//   Serial.println("");
  delay(100);
}

void printMPU6050(Adafruit_MPU6050& mpu) {

  // Serial.println();
  // Serial.println("MPU6050 ------------");

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

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