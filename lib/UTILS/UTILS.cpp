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
  crc ^= 0x55; // Final XOR FOR REASONS (RIP 30 MINS)
  return crc;
}

void LightLEDs(COLOR LED1, COLOR LED2) {
  uint8_t SIPO = 0b00000011;
  SIPO = (SIPO << 3) ^ LED1;
  SIPO = (SIPO << 3) ^ LED2;

  digitalWrite(RCLK, LOW); //lock data
  // Using switches from 2-7, so it has to start with LSB
  shiftOut(SER, SCLK, LSBFIRST, SIPO);
  digitalWrite(RCLK, HIGH); // push data
}

void LoadBufferWithButtonsData(uint8_t* buffer, uint8_t& data_ID) {
  memset(buffer, 0, sizeof(uint8_t) * 32); // Clear buffer before loading new data
  buffer[0] = (bool)!digitalRead(BTN_A); buffer[1] = (bool)!digitalRead(BTN_B);
  buffer[2] = (bool)!digitalRead(BTN_C); buffer[3] = (bool)!digitalRead(BTN_D);
  buffer[4] = (bool)!digitalRead(BTN_E); buffer[5] = (bool)!digitalRead(BTN_F);
  uint16_t joystick = analogRead(JOY_X);
  buffer[6] = joystick & 0xFF; // Lower byte of joystick
  buffer[7] = (joystick >> 8) & 0xFF; // Upper byte of joystick
  joystick = analogRead(JOY_Y);
  buffer[8] = joystick & 0xFF; // Lower byte of joystickY
  buffer[9] = (joystick >> 8) & 0xFF; // Upper byte of joystickY
  buffer[29] = BUTTONS_DATA;
  buffer[30] = data_ID;
  buffer[31] = calculate_CRC8(buffer, 31);
}

bool WasButtonPressed(int Button, bool& wasPressed) {
  if (digitalRead(Button) == HIGH && wasPressed) {
    wasPressed = false;
    return true;
  }
  else if (digitalRead(Button) == LOW) {
    wasPressed = true;
  }
  return false;
}

void DisplayData(uint8_t* buffer) {
  Serial.println(F("Buffer contents:"));
  for (int i = 0; i < 32; i++) {
    Serial.print((int)buffer[i]);
    Serial.print(", ");
  }
  Serial.println();
}

int8_t FloatToUint8(float& value) {
    value *= 10.0f;
    return (int8_t)value;
}




// void LoadBufferWithSensorData(uint8_t* buffer, Sensors& sensors, uint8_t& data_ID) {
//   LoadQMC5883L(sensors.compass, buffer); // Loads compass data into first 9 bytes of buffer

// }



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