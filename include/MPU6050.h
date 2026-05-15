#ifndef MPU6050_H
#define MPU6050_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <QMC5883LCompass.h>



void initializeMPU6050(Adafruit_MPU6050& mpu);
void initializeBMP180(Adafruit_BMP085& bmp);
void initializeQMC5883L(QMC5883LCompass& compass);

void printQMC5883L(QMC5883LCompass& compass);
void printMPU6050(Adafruit_MPU6050& mpu);
void printBMP180(Adafruit_BMP085& bmp);

#endif // MPU6050_H