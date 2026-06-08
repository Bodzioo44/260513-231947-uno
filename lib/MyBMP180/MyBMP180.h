#ifndef MYBMP180_H
#define MYBMP180_H

#include <Adafruit_BMP085.h>

// 16 Bytes of BMP data
struct BMP180Data {
  float Temp;
  int32_t Pressure;
  float Altitude;
  int32_t SeaLevelPressure; 
} __attribute__((packed));

class MyBMP180 {
    private:
        Adafruit_BMP085 _base;

    public:
        void initialize();
        BMP180Data readBMP180FromSensor();
        BMP180Data readBMP180FromBuffer(uint8_t* buffer);
        void loadBMP180ToBuffer(BMP180Data& data, uint8_t* buffer);

};

#endif // MYBMP180_H