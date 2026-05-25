#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <Arduino.h>


#define JOY_X A0
#define JOY_Y A1
#define BTN_A 2
#define BTN_B 3
#define BTN_C 4
#define BTN_D 5
#define BTN_E 6
#define BTN_F 7


#define SER 6
#define RCLK 7
#define SCLK 8


// struct Sensors {
//   Adafruit_MPU6050 mpu;
//   QMC5883LCompass compass;
//   Adafruit_BMP085 bmp;

//   Sensors() {
//     initializeMPU6050(mpu);
//     mpu.setI2CBypass(true);
//     initializeQMC5883L(compass);
//     initializeBMP180(bmp);
//   }
// };

// 18 Bytes of sensor data
// struct SensorData {
//   uint16_t magX; // 2
//   uint16_t magY; // 2
//   uint16_t magZ; // 2
//   char direction[3]; // 3
//   uint8_t accelX; // 1
//   uint8_t accelY; // 1
//   uint8_t accelZ; // 1
//   uint8_t gyroX; // 1
//   uint8_t gyroY; // 1
//   uint8_t gyroZ; // 1
//   uint8_t temperature; // 1
//   uint16_t pressure; // 2


// } __attribute__((packed));

// // 10 Bytes of button data
// struct ButtonsData {
//   bool ButtonA;
//   bool ButtonB;
//   bool ButtonC;
//   bool ButtonD;
//   bool ButtonE;
//   bool ButtonF;
//   uint16_t joystickX;
//   uint16_t joystickY;
// } __attribute__((packed));

enum DATA_TYPE {
  BUTTONS_DATA = 0x01,
  SENSOR_DATA = 0x02,
  RADAR_DATA = 0x03
};

// TODO: assign right values
enum COLOR {
  RED = 0b110,
  GREEN = 0b101,
  BLUE = 0b011,
  WHITER = 0b000,
  CYAN = 0b001,
  MAGENTA = 0b010,
  YELLOW = 0b100,
  OFF = 0b111
};

int freeRam();

int8_t FloatToUint8(float& value);

uint8_t calculate_CRC8(uint8_t* data, int size);

void DisplayData(uint8_t* buffer);
void LoadBufferWithButtonsData(uint8_t* buffer, uint8_t& data_ID);
void LightLEDs(COLOR LED1, COLOR LED2);

bool WasButtonPressed(int Button, bool& wasPressed);

#endif // UTILS_H