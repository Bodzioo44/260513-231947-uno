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

  radio.writeAckPayload(1, &reply_data, sizeof(reply_data));
}

void loop() {
  if (radio.available()) {
    radio.read(&buffer, sizeof(buffer));

    DisplayData(buffer);

    reply_data++;
    Serial.println("Sending ACK payload back to transmitter: ");
    radio.writeAckPayload(1, &buffer, sizeof(buffer));
    
    last_radio_response = millis();
  }

  LoadQMC5883L(compass, buffer);
  LoadMPU6050(mpu, buffer + 9);
  LoadBMP180(bmp, buffer + 15);

  ButtonsData buttons = ReadButtonsData(buffer);

  RadarScan(myservo, radar_data, RADARSAMPLES);
  delay(5000);
  // Forward
  if (buttons.ButtonA) {
    digitalWrite(forward_left, HIGH);
    digitalWrite(forward_right, HIGH);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(BLUE, BLUE);
  }
  // Right
  else if (buttons.ButtonB) {
    digitalWrite(forward_left, HIGH);
    digitalWrite(forward_right, LOW);

    analogWrite(PWM_left, 255);
    analogWrite(PWM_right, 255);
    LightLEDs(MAGENTA, MAGENTA);
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

    LightLEDs(GREEN, GREEN);
  }
  else {
    analogWrite(PWM_left, 0);
    analogWrite(PWM_right, 0);
    // Serial.println("WHYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY");
  }
  // Connection lose protection
  if (millis()-last_radio_response > 1000) {
    analogWrite(PWM_left, 0);
    analogWrite(PWM_right, 0);

    LightLEDs(RED,BLUE);
  }

  // if (i % 4 == 0) {
  //   analogWrite(PWM_left, 0);
  //   analogWrite(PWM_right, 255);
  //   Serial.println("LEFT STOP, RIGHT GO");
  // }
  // else if (i % 4 == 1) {
  //   analogWrite(PWM_left, 255);
  //   analogWrite(PWM_right, 0);
  //   Serial.println("LEFT GO, RIGHT STOP");

  // }
  // else if (i % 4 == 2) {
  //   analogWrite(PWM_left, 255);
  //   analogWrite(PWM_right, 255);
  //   Serial.println("LEFT GO, RIGHT GO");

  // }
  //  else if (i % 4 == 3) {
  //   analogWrite(PWM_left, 0);
  //   analogWrite(PWM_right, 0);
  //   Serial.println("LEFT STOP, RIGHT STOP");
  // }

  // digitalWrite(forward_left, HIGH);
  // digitalWrite(forward_right, HIGH);

  // delay(1000);

  // digitalWrite(forward_left, LOW);
  // digitalWrite(forward_right, LOW);

  // delay(1000);

  // // printMPU6050(mpu);
  // // printQMC5883L(compass);
  // // printBMP180(bmp);


  // i++;
}

// void loop() {
//   x = -1; y = -1; a = -1; b = -1; c= -1; d = -1; dataToRead[6] = 0;
//   if (radio.available()) {
//     Serial.println("Successfull radio read");
//     radio.read(&dataToRead, sizeof(dataToRead));

//     x = dataToRead[0]; y = dataToRead[1];
//     a = dataToRead[2]; b = dataToRead[3];
//     c = dataToRead[4]; d = dataToRead[5];
//     control_data = dataToRead[6];

//     Serial.println("Control data: " + String(control_data));
    

//     if (control_data != (int)calculate_CRC8(dataToRead, 6)) {
//       Serial.println("Control data missmatch!!! abandon ship!!!");
//       // radio.powerDown(); 
//       // delay(100); 
//       // radio.powerUp(); 
//       // delay(100);

//       // radio.begin();
//       // radio.setDataRate(RF24_250KBPS);
//       // radio.openReadingPipe(0, pipeOut);
//       // radio.setPALevel(RF24_PA_MIN);
//       // radio.startListening();

//       return;
//     }
//     Serial.println("Control data check passed");

//     Serial.print("Received: ");
//     for (int i = 0; i < 7; i++) {

//       Serial.print(dataToRead[i]);
//       Serial.print(", ");
//     }
//     if (a == 0){
//       Serial.println("TURNING FRONT");
//       digitalWrite(forward_left, HIGH);
//       digitalWrite(forward_right, HIGH);
//       digitalWrite(backward_left, LOW);
//       digitalWrite(backward_right, LOW);
//     }
//     else if (c == 0){
//       Serial.println("TURNING BACK");
//       digitalWrite(forward_left, LOW);
//       digitalWrite(forward_right, LOW);
//       digitalWrite(backward_left, HIGH);
//       digitalWrite(backward_right, HIGH);
//     }
//     else if (d == 0){
//       Serial.println("TURNING LEFT");
//       digitalWrite(forward_left, LOW);
//       digitalWrite(forward_right, HIGH);
//       digitalWrite(backward_left, HIGH);
//       digitalWrite(backward_right, LOW);    
//     }
//     else if (b == 0){
//       Serial.println("TURNING RIGHT");
//       digitalWrite(forward_left, HIGH);
//       digitalWrite(forward_right, LOW);
//       digitalWrite(backward_left, LOW);
//       digitalWrite(backward_right, HIGH); 
//     }
//     else{
//       Serial.println("TURNING OFF");
//       digitalWrite(forward_left, LOW);
//       digitalWrite(forward_right, LOW);
//       digitalWrite(backward_left, LOW);
//       digitalWrite(backward_right, LOW); 
//     }
//     oopsie_counter = 0;
//   }
//   else {
//     oopsie_counter++;
//     if (oopsie_counter == 100) {
//       Serial.println("TURNING OFF");
//       Serial.println("critical failure, radio absent for 1000ms");
//       digitalWrite(forward_left, LOW);
//       digitalWrite(forward_right, LOW);
//       digitalWrite(backward_left, LOW);
//       digitalWrite(backward_right, LOW); 
//       oopsie_counter = 0;
      
//     }
//     delay(delay_val);
//   }
// }
