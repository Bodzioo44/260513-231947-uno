#ifndef MPU6050_H
#define MPU6050_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <QMC5883LCompass.h>

#include "UTILS.h"

void initializeMPU6050(Adafruit_MPU6050& mpu);
void initializeBMP180(Adafruit_BMP085& bmp);
void initializeQMC5883L(QMC5883LCompass& compass);


MPU6050Data ReadMPU6050(Adafruit_MPU6050& mpu);

void LoadMPU6050(MPU6050Data& data, uint8_t* buffer);

// void LoadQMC5883L(QMC5883LCompass& compass, uint8_t* buffer);

// void LoadBMP180(Adafruit_BMP085& bmp, uint8_t* buffer);


// Debug functions
void printBMP180(Adafruit_BMP085& bmp);
void printMPU6050(Adafruit_MPU6050& mpu);
void printQMC5883L(QMC5883LCompass& compass);

#endif // MPU6050_H