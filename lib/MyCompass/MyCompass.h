#ifndef MYCOMPASS_H
#define MYCOMPASS_H

#include <QMC5883LCompass.h>

// enum class MODE : uint8_t {
//     TX,
//     RX
// };

// struct CompassOffsets {
//     float baseline[3];
//     float magOffsets[3];
//     float magMapping[3][3];
//     float declination;
// };

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
        // void setOffsets(CompassOffsets offsets);
    private:
        // CompassOffsets _offsets;
        // MODE _mode;
        // float _minY, _maxY, _minZ, _maxZ;
        float offset_y, offset_z;
        float amplitude_y, amplitude_z;


};

#endif // MYCOMPASS_H