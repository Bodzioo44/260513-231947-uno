#ifndef MYMPU6050_H
#define MYMPU6050_H

#include <Adafruit_MPU6050.h>

// 28 bytes of data
struct MPU6050Data {
  float Ax;
  float Ay;
  float Az;
  float Gx;
  float Gy;
  float Gz;
  float Temp;
} __attribute__((packed));

class MyMPU6050{
    private:
        Adafruit_MPU6050 _base;
        MPU6050Data _calibrationData;

    public:
        MyMPU6050() : _base() {};
        void initialize();
        MPU6050Data readMPU6050FromSensor();
        MPU6050Data readMPU6050FromBuffer(uint8_t* buffer);
        void loadMPU6050ToBuffer(MPU6050Data& data, uint8_t* buffer);
        void calibrate();
        void print();
        float getAccelY();

};


#endif // MYMPU6050_H