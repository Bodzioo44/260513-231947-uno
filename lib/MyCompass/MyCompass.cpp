#include "MyCompass.h"

void MyCompass::initialize() {
    this->init();

    // _mode = mode;
    // _offsets = offsets;

    Serial.println(F("QMC5883L Found!"));
    // Serial.println(F("Calculating baseline..."));
    // long total_x = 0, total_y = 0, total_z = 0;
    // int samples = 50;
    
    // for (int i = 0; i < samples; i++) {
    //     this->read();
    //     total_x += this->getX();
    //     total_y += this->getY();
    //     total_z += this->getZ();
    //     delay(15);
    // }
    // Serial.println(F("Baseline calculated!"));
    // _offsets.baseline[0] = total_x / (float)samples;
    // _offsets.baseline[1] = total_y / (float)samples;
    // _offsets.baseline[2] = total_z / (float)samples;

    // Serial.print(F("Baseline X: ")); Serial.println(_offsets.baseline[0]);
    // Serial.print(F("Baseline Y: ")); Serial.println(_offsets.baseline[1]);
    // Serial.print(F("Baseline Z: ")); Serial.println(_offsets.baseline[2]);
}

void MyCompass::calibrate() {
    Serial.println(F("Starting calibration..."));
    Serial.println(F("Spin the compass slowly in all directions for about 10 seconds..."));
    float minY = 9999, maxY = -9999;
    float minZ = 9999, maxZ = -9999;

    for (int i = 0; i < 100; i++) {
        this->read();
        int y = this->getY();
        int z = this->getZ();

        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
        if (z < minZ) minZ = z;
        if (z > maxZ) maxZ = z;

        delay(100);
    }

    offset_y = (maxY + minY) / 2.0;
    offset_z = (maxZ + minZ) / 2.0;

    amplitude_y = (maxY - minY) / 2.0;
    amplitude_z = (maxZ - minZ) / 2.0;

    Serial.println(F("Calibration complete!"));
}
// #ifdef RX
CompassData MyCompass::readCompassFromSensor() {
    CompassData data;
    this->read();

    float raw_x = this->getX();
    float raw_y = this->getY();
    float raw_z = this->getZ();

    float y_centered = raw_y - offset_y;
    float z_centered = raw_z - offset_z;

    if (amplitude_y > 0) y_centered /= amplitude_y;
    if (amplitude_z > 0) z_centered /= amplitude_z;

    float heading = atan2(-y_centered, z_centered);
    heading += 0.1105;

    float head_degrees = heading * 180.0 / PI;
    head_degrees += 100.0;   

    data.x = raw_x;
    data.y = raw_y;
    data.z = raw_z;

    if (head_degrees < 0)    head_degrees += 360.0;
    if (head_degrees >= 360.0) head_degrees -= 360.0;

    data.a = head_degrees;

    this->getDirection(data.Direction, data.a);
    return data;
}
// #endif

// CompassData MyCompass::readCompassFromSensor() {
//     CompassData data;
//     this->read();

//     float raw_x = (this->getX() - _offsets.baseline[0]) / 5.0;
//     float raw_y = (this->getY() - _offsets.baseline[1]) / 5.0;
//     float raw_z = (this->getZ() - _offsets.baseline[2]) / 5.0;

//     float x_centered = raw_x - _offsets.magOffsets[0];
//     float y_centered = raw_y - _offsets.magOffsets[1];
//     float z_centered = raw_z - _offsets.magOffsets[2];

//     data.x = (x_centered * _offsets.magMapping[0][0]) + (y_centered * _offsets.magMapping[0][1]) + (z_centered * _offsets.magMapping[0][2]);
//     data.y = (x_centered * _offsets.magMapping[1][0]) + (y_centered * _offsets.magMapping[1][1]) + (z_centered * _offsets.magMapping[1][2]);
//     data.z = (x_centered * _offsets.magMapping[2][0]) + (y_centered * _offsets.magMapping[2][1]) + (z_centered * _offsets.magMapping[2][2]);

//     // Krok 4: Obliczenie Azymutu w radianach i przejście na stopnie
//     float heading = atan2(data.y, data.z);
    
//     // Opcjonalnie: dodanie deklinacji magnetycznej
//     heading += _offsets.declination;

//     data.a = heading * 180.0 / PI; 

//     // Normalizacja wyniku do zakresu 0 - 360 stopni
//     if (data.a < 0) {
//         data.a += 360.0;
//     }
//     if (data.a >= 360.0) {
//         data.a -= 360.0;
//     }
    
//     this->getDirection(data.Direction, data.a);
//     return data;
// }

CompassData MyCompass::readCompassFromBuffer(uint8_t* buffer) {
    CompassData data;
    memcpy(&data, buffer, sizeof(CompassData));
    return data;
}

void MyCompass::loadCompassToBuffer(CompassData& data, uint8_t* buffer) {
    memcpy(buffer+3, &data, sizeof(CompassData));
}

// void MyCompass::setOffsets(CompassOffsets offsets) {
//     _offsets = offsets;
// }