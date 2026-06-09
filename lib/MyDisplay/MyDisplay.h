#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include <Adafruit_PCD8544.h>
#include <Adafruit_GFX.h>

#include "UTILS.h"
#include "MyCompass.h"
#include "MyMPU6050.h"
#include "MyBMP180.h"

#define DC A3
#define CE A2
#define RST 8

class MyDisplay {
    private:
        Adafruit_PCD8544 _base;

    public:
        MyDisplay() : _base(DC, CE, RST) {};
        void initialize();
        void displayButtons(ButtonsData& data);
        void displayGyro(MPU6050Data& data, bool& TX_screen);
        void displayAccel(MPU6050Data& data, bool& TX_screen);
        void displayBaro(BMP180Data& data, bool& TX_screen);
        void displayMag(CompassData& data, bool& TX_screen);
        void displaySpeed(SpeedData& data, bool& TX_screen);
        void displayRadar(RadarData& data);
        void displayEcho(RadarData& data);
        // void displaySpeedCalibrate();

        void message(const char* msg) {
            this->_base.clearDisplay();
            this->_base.setCursor(0,0);
            this->_base.println(msg);
            this->_base.display();
        }
        void message(const __FlashStringHelper* msg) {
            this->_base.clearDisplay();
            this->_base.setCursor(0,0);
            this->_base.println(msg);
            this->_base.display();
        }
};


#endif // MYDISPLAY_H