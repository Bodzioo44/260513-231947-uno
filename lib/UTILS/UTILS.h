#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <Arduino.h>
#include <Servo.h>

//////////////////////
// TRANSMITTER DEFINES
//////////////////////

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

///////////////////
// RECEIVER DEFINES
///////////////////
#define servoPin A2
#define EchoPin A1
#define TrigPin A0

#define RADARSAMPLES 20



enum class DATA_TYPE : uint8_t {
  BUTTONS_DATA_TX,
  MPU6050_DATA_TX,
  BMP180_DATA_TX,
  QMCL588L_DATA_TX,

  MPU6050_DATA_RX,
  BMP180_DATA_RX,
  QMCL588L_DATA_RX,
  RADAR_DATA_RX,

  NONE
};

enum class COLOR : uint8_t {
  RED = 0b110,
  GREEN = 0b101,
  BLUE = 0b011,
  WHITER = 0b000,
  CYAN = 0b001,
  MAGENTA = 0b010,
  YELLOW = 0b100,
  OFF = 0b111
};

struct Header {
  DATA_TYPE DataType; 
  DATA_TYPE RequestedData;
  uint8_t TransmissionID;
} __attribute__((packed));

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

// 28 Bytes of MPU data
// Questionable, might need reduced precision later, or split payloads
struct MPU6050Data {
  float Ax;
  float Ay;
  float Az;
  float Gx;
  float Gy;
  float Gz;
  float Temp;
} __attribute__((packed));

// 16 Bytes of BMP data
struct BMP180Data {
  float Temp;
  int32_t Pressure;
  float Altitude;
  int32_t SeaLevelPressure; 
} __attribute__((packed));

// 11 Bytes of QMCL data
struct QMCL588LData {
  int x;
  int y;
  int z;
  int a;
  char Direction[3];
} __attribute__((packed));

int freeRam();
uint8_t calculate_CRC8(uint8_t* data, int size);

void LightLEDs(COLOR LED1, COLOR LED2);
bool WasButtonPressed(int Button, bool& wasPressed);

void DisplayData(uint8_t* buffer);



void RadarScan(Servo servo, uint8_t* radar_data, uint8_t samples);
int GetDistance();



void LoadBufferWithButtonsData(uint8_t* buffer, ButtonsData& data);
ButtonsData ReadButtons();
ButtonsData ReadButtonsDataFromBuffer(uint8_t* buffer);


Header ReadHeader(uint8_t* buffer);
void LoadHeader(uint8_t* buffer, Header& header);





#endif // UTILS_H