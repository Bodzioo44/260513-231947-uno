#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <RF24.h>
#include <Wire.h>

#include "MPU6050.h"

#define JOY_X A0
#define JOY_Y A1
#define BTN_A 2
#define BTN_B 3
#define BTN_C 4
#define BTN_D 5
#define BTN_E 6
#define BTN_F 7

// Software SPI, Doesnt work with multiple SPI chips
// Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, A2, A3, A4);

// Hardware SPI: D/C A2, CE 8, RST A3
Adafruit_PCD8544 display = Adafruit_PCD8544(A2, 8, A3);
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;
QMC5883LCompass compass;

RF24 radio(9, 10); // CE, CSN

const uint64_t pipeOut = 0xA4D5C6F7E1LL;
int dataToSend[7]; 

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void display_freeram() {
  Serial.print(F("Free RAM: "));
  Serial.println(freeRam());
}


void setup() {
  Serial.begin(9600);

  initializeMPU6050(mpu);
  mpu.setI2CBypass(true);

  initializeQMC5883L(compass);
  initializeBMP180(bmp);
  

  display.begin();
  display.setContrast(55);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  // display.setRotation(2);
  // display.println("INITIALIZING");
  // display.print("RADIO: ");
  // display.display();

  if (!radio.begin()) {
    display.clearDisplay();
    display.println(F("FAILED TO INITIALIZE RADIO!"));
    display.println(F("CHECK RECEIVER"));
    display.display();
    while (1); 
  }
  radio.openWritingPipe(pipeOut);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening(); 

  pinMode(BTN_A, INPUT);
  pinMode(BTN_B, INPUT);
  pinMode(BTN_C, INPUT);
  pinMode(BTN_D, INPUT);
  pinMode(BTN_E, INPUT);
  pinMode(BTN_F, INPUT);

  display.println(F("READY..."));
  display.display();

  dataToSend[6] = -1;
  
  delay(1000);
}

void loop() {
  dataToSend[0] = analogRead(JOY_X); dataToSend[1] = analogRead(JOY_Y);
  dataToSend[2] = digitalRead(BTN_A); dataToSend[3] = digitalRead(BTN_B); 
  dataToSend[4] = digitalRead(BTN_C); dataToSend[5] = digitalRead(BTN_D);

  bool success = radio.write(&dataToSend, sizeof(dataToSend));

  // Serial.print("Link: "); 
  // Serial.println(success ? "OK" : "LOST");

  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("- DEBUG MODE -"));
  
  display.print(F("X: ")); display.println(dataToSend[0]);
  display.print(F("Y: ")); display.println(dataToSend[1]);
  
  display.print(F("Link: ")); 
  display.println(success ? F("OK") : F("LOST"));

  display.print(F("Btns: "));
  if(dataToSend[2] == LOW) display.print(F("A "));
  if(dataToSend[3] == LOW) display.print(F("B "));
  if(dataToSend[4] == LOW) display.print(F("C "));
  if(dataToSend[5] == LOW) display.print(F("D "));
  display.display();

  printBMP180(bmp);
  printMPU6050(mpu);
  printQMC5883L(compass);
  display_freeram();

  delay(100); 
}
