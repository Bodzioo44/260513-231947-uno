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
  uint8_t SIPO = 0;
  SIPO = (SIPO << 3) ^ (uint8_t)LED1;
  SIPO = (SIPO << 3) ^ (uint8_t)LED2;

  digitalWrite(RCLK, LOW); //lock data
  // Using switches from 2-7, so it has to start with LSB
  shiftOut(SER, SCLK, LSBFIRST, SIPO);
  digitalWrite(RCLK, HIGH); // push data
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

Header ReadHeader(uint8_t* buffer) {
  Header header;
  memcpy(&header, buffer, sizeof(header));
  return header;
}

void LoadHeader(uint8_t* buffer, Header& header) {
  memcpy(buffer, &header, sizeof(header));
}

void LoadCRC8(uint8_t* buffer_start, uint8_t* buffer) {
  *buffer = calculate_CRC8(buffer_start, 31);
}

void LoadBufferWithButtonsData(uint8_t* buffer, ButtonsData& data) {
  memcpy(buffer+3, &data, sizeof(data));
}

ButtonsData ReadButtons() {
  ButtonsData data;
  data.ButtonA = (bool)!digitalRead(BTN_A); data.ButtonB = (bool)!digitalRead(BTN_B);
  data.ButtonC = (bool)!digitalRead(BTN_C); data.ButtonD = (bool)!digitalRead(BTN_D);
  data.ButtonE = (bool)!digitalRead(BTN_E); data.ButtonF = (bool)!digitalRead(BTN_F);
  data.joystickX = analogRead(JOY_X); data.joystickY = analogRead(JOY_Y);
  return data;
}

ButtonsData ReadButtonsDataFromBuffer(uint8_t* buffer) {
  ButtonsData data;
  memcpy(&data, buffer+3, sizeof(data));
  return data;
}


int SpecialRadarScan(Servo servo, int angle) {
  servo.write(angle);
  int pomiar = GetDistance();
  if (pomiar < 0 || pomiar > 150) {pomiar = 150;}
  return pomiar;

}

RadarData RadarScan(Servo servo, uint8_t samples = RADARSAMPLES) {
  int total_range = RADARANGLE;
  int bottom_range = (180-total_range)/2+RADAROFFSET;
  int top_range = total_range+bottom_range;
  int delay_val = 500;
  int pomiar;

  int i = 0;

  RadarData data;

  servo.write(bottom_range);

  // Serial.print("Skan od: "); Serial.println(bottom_range);
  // Serial.print("Skan do: "); Serial.println(top_range);
  // Serial.print("Skan co: "); Serial.println(total_range/samples);
  int skok = RADARANGLE/RADARSAMPLES;

  for (int i = 0; i < RADARSAMPLES; i++) {
    servo.write(bottom_range+skok*i); 
    delay(delay_val);        
    //Serial.println(pos);      
    pomiar = GetDistance();
    if (pomiar < 0 || pomiar > 150) {pomiar = 150;}
    Serial.print("Pomiar nr: "); Serial.print(i); Serial.print(", odleglosc cm: "); Serial.println(pomiar);
    data.samples[i] = pomiar;
  }

  servo.write(total_range/2+bottom_range);
  return data;
}

int GetDistance() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  int duration = pulseIn(EchoPin, HIGH);
  int distance = (duration*.0343)/2;

  return distance;
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