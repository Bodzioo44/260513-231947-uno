#ifndef MPU6050_H
#define MPU6050_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <QMC5883LCompass.h>

void initializeMPU6050(Adafruit_MPU6050& mpu);
void initializeBMP180(Adafruit_BMP085& bmp);
void initializeQMC5883L(QMC5883LCompass& compass);

void LoadQMC5883L(QMC5883LCompass& compass, uint8_t* buffer);
void printMPU6050(Adafruit_MPU6050& mpu);
void printBMP180(Adafruit_BMP085& bmp);


// class Compass : public QMC5883LCompass {
//   public:
//     Compass() : QMC5883LCompass() {}
//     void init() {
//       QMC5883LCompass::init();
//       QMC5883LCompass::setCalibrationOffsets(105.00, 130.00, -677.00);
//       QMC5883LCompass::setCalibrationScales(1.01, 0.86, 1.18);
//     }
//     void read() {
//       QMC5883LCompass::read();
//       int x, y, z, a;
//       char myArray[3];

//         x = QMC5883LCompass::getX();
//         y = QMC5883LCompass::getY();
//         z = QMC5883LCompass::getZ();
//         a = QMC5883LCompass::getAzimuth();

    

//     }
// };

// class MPU6050 : public Adafruit_MPU6050 {
//   public:
  
// };

// class BMP180 : public Adafruit_BMP085 {
//   public:
//     BMP180() : Adafruit_BMP085() {}
//     bool begin() {
//       return Adafruit_BMP085::begin();
//     }
//     float readTemperature() {
//       return Adafruit_BMP085::readTemperature();
//     }
//     int32_t readPressure() {
//       return Adafruit_BMP085::readPressure();
//     }
// };




#endif // MPU6050_H