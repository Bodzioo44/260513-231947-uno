#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <Arduino.h>

enum class DATA_TYPE : uint8_t {
  BUTTONS_DATA_TX,
  MPU6050_DATA_TX,
  BMP180_DATA_TX,
  QMC5883L_DATA_TX,

  MPU6050_DATA_RX,
  BMP180_DATA_RX,
  QMC5883L_DATA_RX,
  RADAR_DATA_RX,
  SPEEEED_DATA_RX,
  CALIBRATE_SPEED_RX,

  NONE
};

struct Header {
  DATA_TYPE DataType; 
  DATA_TYPE RequestedData;
  uint8_t TransmissionID;
} __attribute__((packed));

template<typename T>
struct Packet {
  // DATA_TYPE dataType;
  // DATA_TYPE requestedData;
  // uint8_t transmissionID;
  Header header;
  T data;
  uint8_t crc8;
} __attribute__((packed));

template<typename T> 
void LoadBufferWithData(uint8_t* buffer, T data);
template <typename T>
T ReadDataFromBuffer(uint8_t* buffer);

//////////
// HEADER
//////////



Header ReadHeader(uint8_t* buffer);
void LoadHeader(uint8_t* buffer, Header& header);

//////////////
// RADAR DATA
//////////////

#define RADARSAMPLES 28
#define RADARANGLE 120
#define RADAROFFSET -10

struct RadarData {
  uint8_t samples[RADARSAMPLES];
} __attribute__((packed));


RadarData ReadRadarDataFromBuffer(uint8_t* buffer);
void LoadBufferWithRadardData(uint8_t* buffer, RadarData& data);

///////////
// BUTTONS
///////////

// 10 Bytes of button data
struct ButtonsData {
  bool ButtonA;
  bool ButtonB;
  bool ButtonC;
  bool ButtonD;
  bool ButtonE;
  bool ButtonF;
  uint16_t joystickX;
  uint16_t joystickY;
} __attribute__((packed));

ButtonsData ReadButtonsDataFromBuffer(uint8_t* buffer);
void LoadBufferWithButtonsData(uint8_t* buffer, ButtonsData& data);

/////////////////
// SPEEEEED DATA
/////////////////

// 22 bytes
struct SpeedData {
  float current_acceleration;
  float current_velocity;
  float current_distance;
  float heading_radians;
  float delta_x;
  float delta_y;
} __attribute__((packed));

SpeedData ReadSpeedDataFromBuffer(uint8_t* buffer);
void LoadBufferWithSpeedData(uint8_t* buffer, SpeedData& data);

//////////
// UTILS
//////////


int freeRam();
void DisplayData(uint8_t* buffer);
uint8_t calculate_CRC8(uint8_t* data, int size);


#endif // UTILS_H