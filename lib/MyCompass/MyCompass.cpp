#include "MyCompass.h"

void MyCompass::initialize() {
    this->init();
    Serial.println(F("QMC5883L Found!"));
}

void MyCompass::calibrate() {
    Serial.println(F("Starting compass calibration..."));
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

    if (head_degrees < 0) head_degrees += 360.0;
    if (head_degrees >= 360.0) head_degrees -= 360.0;

    data.a = head_degrees;

    Serial.println(F("Heading: ")); Serial.println(data.a);

    this->getDirection(data.Direction, data.a);
    return data;
}

CompassData MyCompass::readCompassFromBuffer(uint8_t* buffer) {
    CompassData data;
    memcpy(&data, buffer+3, sizeof(CompassData));
    return data;
}

void MyCompass::loadCompassToBuffer(CompassData& data, uint8_t* buffer) {
    memcpy(buffer+3, &data, sizeof(CompassData));
}
