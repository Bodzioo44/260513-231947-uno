#include "UTILS.h"

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
  crc ^= 0x55;
  return crc;
}

//////////
// TEST???
//////////

template<typename T>
inline void LoadBufferWithData(uint8_t* buffer, T data) {
  memcpy(buffer+3, &data , sizeof(data));
}

template<typename T>
inline T ReadDataFromBuffer(uint8_t* buffer) {
  T data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

Header ReadHeader(uint8_t* buffer) {
  Header header;
  memcpy(&header, buffer, sizeof(header));
  return header;
}

inline void LoadHeader(uint8_t* buffer, Header& header) {
  memcpy(buffer, &header, sizeof(header));
}

inline void LoadCRC8(uint8_t* buffer_start, uint8_t* buffer) {
  *buffer = calculate_CRC8(buffer_start, 31);
}

void LoadBufferWithButtonsData(uint8_t* buffer, ButtonsData& data) {
  memcpy(buffer+3, &data, sizeof(data));
}



ButtonsData ReadButtonsDataFromBuffer(uint8_t* buffer) {
  ButtonsData data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

void DisplayData(uint8_t* buffer) {
  Serial.println(F("Buffer contents:"));
  for (int i = 0; i < 32; i++) {
    Serial.print((int)buffer[i]);
    Serial.print(", ");
  }
  Serial.println();
}


SpeedData ReadSpeedDataFromBuffer(uint8_t* buffer) {
  SpeedData data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

void LoadBufferWithSpeedData(uint8_t* buffer, SpeedData& data) {
  memcpy(buffer+3, &data, sizeof(data));
}


RadarData ReadRadarDataFromBuffer(uint8_t* buffer) {
  RadarData data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}

void LoadBufferWithRadardData(uint8_t* buffer, RadarData& data) {
  memcpy(buffer+3, &data, sizeof(data));
}