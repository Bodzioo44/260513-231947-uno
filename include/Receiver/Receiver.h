#ifndef RECEIVER_H
#define RECEIVER_H

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>


#include "MyCompass.h"
#include "MyMPU6050.h"
#include "MyBMP180.h"
#include "UTILS.h"

// SR-HC04 and Servo pins
#define servoPin A2
#define EchoPin A1
#define TrigPin A0

// Shift register pins
#define SER 6
#define RCLK 7
#define SCLK 8

// Motor controls
// forward HIGH means it spins forward, LOW means it spins backwards
// for full stop change PMW to 0
#define PWM_left 5
#define PWM_right 3
#define forward_left 4
#define forward_right 2

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


// TX requests specific portion of the radar? or just let it blast UDP style

class MySonar {
  public:
    MySonar() {
      servo.attach(servoPin);
      pinMode(TrigPin, OUTPUT);
      pinMode(EchoPin, INPUT);
    }


  private:
    bool a;
    Servo servo;
    int GetDistance();
    
};

void setup();
void loop();


RadarData RadarScan(Servo servo, uint8_t samples = RADARSAMPLES);
void LightLEDs(COLOR LED1, COLOR LED2);




#endif