#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

// #include "MPU6050.h"
#include "MyCompass.h"
#include "MyMPU6050.h"
#include "MyBMP180.h"

#include "UTILS.h"

#define PWM_left 5
#define forward_left 4
#define PWM_right 3
#define forward_right 2


Servo myservo;

MyMPU6050 mpu;
MyCompass compass;
MyBMP180 bmp;


RF24 radio(9, 10); // CE, CSN
uint8_t buffer[32]; 
const uint8_t addresses[] = { 0xD4, 0xF6 };
ButtonsData buttons;

int reply_data = 0;
unsigned long last_radio_response = 0;

unsigned long last_measurement = 0;
// unsigned long time_from_stop = 0;

float last_acceleration = 0;
float last_velocity = 0.0f;

float current_velocity = 0;
float current_distance = 0;

uint8_t radar_data[RADARSAMPLES];

void setup() {
  Serial.begin(115200);

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

  mpu.initialize();
  compass.initialize();
  bmp.initialize();

  digitalWrite(RCLK, HIGH); 
  digitalWrite(SER, LOW);
  digitalWrite(SCLK, LOW);

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
  
  // No idea whats happening here, or why its needed
  LightLEDs(COLOR::OFF, COLOR::OFF);
  LightLEDs(COLOR::OFF, COLOR::OFF);
  delay(2500);

  // calibrate mpu6050 for 10 sec
  LightLEDs(COLOR::RED, COLOR::RED);
  mpu.calibrate();

  // calibrate compass for 10 sec
  digitalWrite(forward_left, HIGH);
  digitalWrite(forward_right, LOW);

  analogWrite(PWM_left, 160);
  analogWrite(PWM_right, 160);
  LightLEDs(COLOR::YELLOW, COLOR::YELLOW);
  compass.calibrate();

  // acknowledge calibration complete
  analogWrite(PWM_left, 0);
  analogWrite(PWM_right, 0);
  LightLEDs(COLOR::GREEN, COLOR::GREEN);
  delay(2500);
  LightLEDs(COLOR::OFF, COLOR::OFF);
}

void loop() {
  unsigned long current_time = millis(); // to avoid rollover?

  float dt = (current_time - last_measurement) / 1000.0f;
  last_measurement = current_time;

  float current_acceleration = mpu.getAccelY();
  if (abs(current_acceleration) < 0.1) {
    current_acceleration = 0;
  }
  // else {
  //   Serial.println("WE GOT THEMMMM");
  // }

  current_velocity += ((last_acceleration + current_acceleration) / 2.0f) * dt;
  current_distance += ((last_velocity + current_velocity) / 2.0f) * dt;

  last_acceleration = current_acceleration;
  last_velocity = current_velocity;

  Serial.print("Total Distance: "); Serial.println(current_distance);
  

  Serial.print(F("Current Accel Y: ")); Serial.println(current_acceleration);
  // Serial.print(F("Current Azimuth: ")); Serial.println(compass.getAzimuth());
  Serial.print(F("Current Velocity: ")); Serial.println(current_velocity);


  // mpu.print();
  // delay(50);

  if (radio.available()) {
    radio.read(&buffer, sizeof(buffer));
    Serial.println(F("Data received from TX:"));
    DisplayData(buffer);
    last_radio_response = millis();
  

    // TODO: Get rid of the check? kinda useless since we only send buttons to RX
    Header header = ReadHeader(buffer);
    if (header.DataType == DATA_TYPE::BUTTONS_DATA_TX) {
      buttons = ReadButtonsDataFromBuffer(buffer);
    }

    switch (header.RequestedData) {
      case DATA_TYPE::MPU6050_DATA_RX: {
        header.DataType = DATA_TYPE::MPU6050_DATA_RX;
        MPU6050Data data = mpu.readMPU6050FromSensor();
        mpu.loadMPU6050ToBuffer(data, buffer);
        break;
      }
      case DATA_TYPE::BMP180_DATA_RX: {
        header.DataType = DATA_TYPE::BMP180_DATA_RX;
        BMP180Data data = bmp.readBMP180FromSensor();
        bmp.loadBMP180ToBuffer(data, buffer);
        break;
      }
      case DATA_TYPE::QMC5883L_DATA_RX: {
        header.DataType = DATA_TYPE::QMC5883L_DATA_RX;
        CompassData data = compass.readCompassFromSensor();
        compass.loadCompassToBuffer(data, buffer);
        break;
      }
      case DATA_TYPE::SPEEEED_DATA_RX: {
        header.DataType = DATA_TYPE::SPEEEED_DATA_RX;
        SpeedData data = {
          .current_acceleration = current_acceleration,
          .current_velocity = current_velocity,
          .current_distance = current_distance
        };
        LoadBufferWithSpeedData(buffer, data);
        break;
      }
      default:
        break;
    }

    header.RequestedData = DATA_TYPE::BUTTONS_DATA_TX;
    LoadHeader(buffer, header);
    buffer[31] = calculate_CRC8(buffer, sizeof(buffer)-1);

    Serial.println(F("Sending data to TX (by loading AckPayload)..."));
    DisplayData(buffer);
    bool test = radio.writeAckPayload(1, &buffer, sizeof(buffer));
    Serial.println(test);
  }

  // Forward
  if (buttons.ButtonA) {
    digitalWrite(forward_left, HIGH);
    digitalWrite(forward_right, HIGH);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(COLOR::GREEN, COLOR::GREEN);
  }
  // Right
  else if (buttons.ButtonB) {
    digitalWrite(forward_left, HIGH);
    digitalWrite(forward_right, LOW);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(COLOR::YELLOW, COLOR::YELLOW);
  }
  // BACK
  else if (buttons.ButtonC) {
    digitalWrite(forward_left, LOW);
    digitalWrite(forward_right, LOW);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(COLOR::RED, COLOR::RED);
  }
  // LEFT
  else if (buttons.ButtonD) {
    digitalWrite(forward_left, LOW);
    digitalWrite(forward_right, HIGH);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);

    LightLEDs(COLOR::BLUE, COLOR::BLUE);
  }
  else {
    analogWrite(PWM_left, 0);
    analogWrite(PWM_right, 0);
    LightLEDs(COLOR::OFF, COLOR::OFF);

    current_velocity = 0;

  }
  // Connection lose protection
  // TODO: add additional check from unsigned long overflow?
  if (millis()-last_radio_response > 1000) {
    analogWrite(PWM_left, 0);
    analogWrite(PWM_right, 0);

    LightLEDs(COLOR::RED, COLOR::BLUE);
  }
}
