#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <Arduino.h>

#include "MPU6050.h"

#define JOY_X A0
#define JOY_Y A1
#define BTN_A 2
#define BTN_B 3
#define BTN_C 4
#define BTN_D 5
#define BTN_E 6
#define BTN_F 7


struct Sensors {
  Adafruit_MPU6050 mpu;
  QMC5883LCompass compass;
  Adafruit_BMP085 bmp;

  Sensors() {
    initializeMPU6050(mpu);
    mpu.setI2CBypass(true);
    initializeQMC5883L(compass);
    initializeBMP180(bmp);
  }
};

// 18 Bytes of sensor data
// struct SensorData {
//   uint8_t temperature; // 1
//   uint16_t pressure; // 2
//   uint8_t accelX; // 1
//   uint8_t accelY; // 1
//   uint8_t accelZ; // 1
//   uint8_t gyroX; // 1
//   uint8_t gyroY; // 1
//   uint8_t gyroZ; // 1
//   uint16_t magX; // 2
//   uint16_t magY; // 2
//   uint16_t magZ; // 2
//   char direction[3]; // 3
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

void DisplayData(uint8_t* buffer) {
  Serial.println(F("Buffer contents:"));
  for (int i = 0; i < 32; i++) {
    Serial.print(buffer[i]);
    Serial.print(", ");
  }
  Serial.println();
}



void LoadBufferWithSensorData(uint8_t* buffer, Sensors& sensors, uint8_t& data_ID) {
  LoadQMC5883L(sensors.compass, buffer); // Loads compass data into first 9 bytes of buffer

}

void LoadBufferWithButtonsData(uint8_t* buffer, uint8_t& data_ID) {
  memset(buffer, 0, sizeof(buffer));

  buffer[0] = (bool)digitalRead(BTN_A); buffer[1] = (bool)digitalRead(BTN_B);
  buffer[2] = (bool)digitalRead(BTN_C); buffer[3] = (bool)digitalRead(BTN_D);
  buffer[4] = (bool)digitalRead(BTN_E); buffer[5] = (bool)digitalRead(BTN_F);
  uint16_t joystick = analogRead(JOY_X);
  buffer[6] = joystick & 0xFF; // Lower byte of joystick
  buffer[7] = (joystick >> 8) & 0xFF; // Upper byte of joystick
  joystick = analogRead(JOY_Y);
  buffer[8] = joystick & 0xFF; // Lower byte of joystickY
  buffer[9] = (joystick >> 8) & 0xFF; // Upper byte of joystickY
  buffer[29] = BUTTONS_DATA;
  buffer[30] = data_ID;
  buffer[31] = calculate_CRC8(buffer, 31); // Calculate CRC for
}


// bool SendButtonsData(RF24& radio, const ButtonsData& data, uint8_t& data_ID) {
//   uint8_t buffer[32];
//   memset(buffer, 0, sizeof(buffer));

//   buffer[0] = data.ButtonA; buffer[1] = data.ButtonB;
//   buffer[2] = data.ButtonC; buffer[3] = data.ButtonD;
//   buffer[4] = data.ButtonE; buffer[5] = data.ButtonF;
//   buffer[6] = data.joystickX & 0xFF; // Lower byte of joystickX
//   buffer[7] = (data.joystickX >> 8) & 0xFF; // Upper byte of joystickX
//   buffer[8] = data.joystickY & 0xFF; // Lower byte of joystickY
//   buffer[9] = (data.joystickY >> 8) & 0xFF; // Upper byte of joystickY

//   buffer[29] = SENSOR_DATA;
//   buffer[30] = data_ID;
//   buffer[31] = calculate_CRC8(buffer, 31); // Calculate CRC for the first 31 bytes

//   return radio.write(&buffer, sizeof(buffer));
// }


// Loads Sensor Data into buffer, 
// bool SendSensorData(RF24& radio, const SensorData& data, uint8_t& data_ID) {
//   uint8_t buffer[32];
//   memset(buffer, 0, sizeof(buffer));

//   buffer[0] = data.temperature;
//   buffer[1] = data.pressure & 0xFF; // Lower byte of pressure
//   buffer[2] = (data.pressure >> 8) & 0xFF; // Upper byte of pressure
//   buffer[3] = data.accelX;
//   buffer[4] = data.accelY;
//   buffer[5] = data.accelZ;
//   buffer[6] = data.gyroX;
//   buffer[7] = data.gyroY;
//   buffer[8] = data.gyroZ;
//   buffer[9] = data.magX & 0xFF; // Lower byte of magX
//   buffer[10] = (data.magX >> 8) & 0xFF; // Upper byte of magX
//   buffer[11] = data.magY & 0xFF; // Lower byte of magY
//   buffer[12] = (data.magY >> 8) & 0xFF; // Upper byte of magY
//   buffer[13] = data.magZ & 0xFF; // Lower byte of magZ
//   buffer[14] = (data.magZ >> 8) & 0xFF; // Upper byte of magZ
//   memcpy(&buffer[15], data.direction, sizeof(data.direction)); // Copy direction chars

//   buffer[29] = SENSOR_DATA;
//   buffer[30] = data_ID;
//   buffer[31] = calculate_CRC8(buffer, 31); // Calculate CRC for the first 31 bytes

//   return radio.write(&buffer, sizeof(buffer));
// }

// TODO: assign right values
enum COLOR {
  RED = 0b110,
  GREEN = 0b101,
  BLUE = 0b011,
  WHITER = 0b000,
  CYAN = 0b001,
  MAGENTA = 0b010,
  YELLOW = 0b100,
};

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}


uint8_t calculate_CRC8(uint8_t* data, int size) {
  uint8_t crc = 0x00; // Initial value for CRC
  uint8_t* ptr = (uint8_t*)data; // pointer to the current byte
  int byteCount = size * sizeof(uint8_t); // total number of bytes to process

  for (int i = 0; i < byteCount; i++) {
    crc = crc ^ ptr[i]; // XOR byte into the CRC
    for (int j = 0; j < 8; j++) {
      if (crc & 0b10000000) { // Compares MSB
        crc = (crc << 1) ^ 0x07; // Shift left and XOR with 0x07
      } else { // Skip MSB
        crc <<= 1; // Shift left
      }
    }
  }
  crc ^= 0x55; // Final XOR FOR REASONS (RIP 30 MINS)
  return crc;
}

#define SER 0
#define RCLK 1
#define SCLK 2

void LightLEDs(COLOR LED1, COLOR LED2) {

  uint8_t SIPO = 0b00000011;
  SIPO = (SIPO << 3) ^ LED1;
  SIPO = (SIPO << 3) ^ LED2;

  digitalWrite(RCLK, LOW); //lock data
  // Using switches from 2-7, so it has to start with LSB
  shiftOut(SER, SCLK, LSBFIRST, SIPO);
  digitalWrite(RCLK, HIGH); // push data

}

#endif // UTILS_H