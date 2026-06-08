#ifndef MYCOMPASS_H
#define MYCOMPASS_H

#include <QMC5883LCompass.h>

struct CompassData {
  int x;
  int y;
  int z;
  int a;
  char Direction[3];
} __attribute__((packed));



class MyCompass: public QMC5883LCompass {
    public:
        MyCompass() : QMC5883LCompass() {};
        void initialize();
        void calibrate();
        CompassData readCompassFromSensor();
        CompassData readCompassFromBuffer(uint8_t* buffer);
        void loadCompassToBuffer(CompassData& data, uint8_t* buffer);
        int getAzimuth();
    private:
        float offset_y, offset_z;
        float amplitude_y, amplitude_z;


};

#endif // MYCOMPASS_H