#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

#include "MPU6050.h"
#include "UTILS.h"

#define PWM_left 5
#define forward_left 4
#define PWM_right 3
#define forward_right 2


Servo myservo; 

Adafruit_MPU6050 mpu;
QMC5883LCompass compass;
Adafruit_BMP085 bmp;

RF24 radio(9, 10); // CE, CSN
uint8_t buffer[32]; 
const uint8_t addresses[] = { 0xD4, 0xF6 };
ButtonsData buttons;

int reply_data = 0;
unsigned long last_radio_response;
uint8_t radar_data[RADARSAMPLES];


void setup() {
  Serial.begin(9600);

  pinMode(PWM_left, OUTPUT);
  pinMode(PWM_right, OUTPUT);
  pinMode(forward_left, OUTPUT);
  pinMode(forward_right, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(SER, OUTPUT);

  myservo.attach(servoPin); 
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  analogWrite(PWM_left, 0);
  analogWrite(PWM_right, 0);

  initializeMPU6050(mpu);
  mpu.setI2CBypass(true);
  initializeQMC5883L(compass);
  initializeBMP180(bmp);

  LightLEDs(OFF, OFF);

  if (!radio.begin()) {
    Serial.println(F("FAILED TO INITIALIZE RADIO! CHECK HARDWARE!"));
    while (1); 
  }

  radio.setPALevel(RF24_PA_MIN);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();

  radio.openReadingPipe(1, addresses[0]);

  radio.startListening();

  radio.writeAckPayload(1, &buffer, sizeof(buffer));
  
}

void loop() {
  if (radio.available()) {
    radio.read(&buffer, sizeof(buffer));
    Serial.println(F("Data received from TX"));
    DisplayData(buffer);
    last_radio_response = millis();
  }

  // TODO: Get rid of the check? kinda useless since we only send buttons to RX
  Header header = ReadHeader(buffer);
  if (header.DataType == DATA_TYPE::BUTTONS_DATA_TX) {
    buttons = ReadButtonsData(buffer+sizeof(header));
  }

  switch (header.RequestedData) {
    case DATA_TYPE::MPU6050_DATA_RX: {
      header.DataType = DATA_TYPE::MPU6050_DATA_RX;
      MPU6050Data data = ReadMPU6050(mpu);
      LoadMPU6050(data, buffer+3);
      break;
    }
    case DATA_TYPE::BMP180_DATA_RX: {
      header.DataType = DATA_TYPE::BMP180_DATA_RX;
      BMP180Data data = ReadBMP180(bmp);
      LoadBMP180(data, buffer+3);
      break;
    }
    case DATA_TYPE::QMCL588L_DATA_RX: {
      header.DataType = DATA_TYPE::QMCL588L_DATA_RX;
      QMCL588LData data = ReadQMCL5883L(compass);
      LoadQMCL588L(data, buffer+3);
      break;
    }
  }

  header.RequestedData = DATA_TYPE::BUTTONS_DATA_TX;
  LoadHeader(buffer, header);
  buffer[31] = calculate_CRC8(buffer, sizeof(buffer)-1);

  radio.writeAckPayload(1, &buffer, sizeof(buffer));
    

  // Forward
  if (buttons.ButtonA) {
    digitalWrite(forward_left, HIGH);
    digitalWrite(forward_right, HIGH);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(GREEN, GREEN);
  }
  // Right
  else if (buttons.ButtonB) {
    digitalWrite(forward_left, HIGH);
    digitalWrite(forward_right, LOW);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(YELLOW, YELLOW);
  }
  // BACK
  else if (buttons.ButtonC) {
    digitalWrite(forward_left, LOW);
    digitalWrite(forward_right, LOW);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(RED, RED);
  }
  // LEFT
  else if (buttons.ButtonD) {
    digitalWrite(forward_left, LOW);
    digitalWrite(forward_right, HIGH);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);

    LightLEDs(BLUE, BLUE);
  }
  else {
    analogWrite(PWM_left, 0);
    analogWrite(PWM_right, 0);
    LightLEDs(OFF, OFF);
  }
  // Connection lose protection
  // TODO: add additional check from unsigned long overflow.
  if (millis()-last_radio_response > 1000) {
    analogWrite(PWM_left, 0);
    analogWrite(PWM_right, 0);

    LightLEDs(RED,BLUE);
  }
}
