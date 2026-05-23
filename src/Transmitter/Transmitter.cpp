#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <RF24.h>
#include <Wire.h>

#include "MPU6050.h"
#include "UTILS.h"


// Software SPI, Doesnt work with multiple SPI chips
// Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, A2, A3, A4);

// Hardware SPI: D/C A2, CE 8, RST A3
Adafruit_PCD8544 display = Adafruit_PCD8544(A2, 8, A3);

Sensors sensors;

RF24 radio(9, 10); // CE, CSN

const uint64_t pipeOut = 0xA4D5C6F7E1LL;

uint8_t buffer[32]; 
uint8_t data_id = 0;

void setup() {
  pinMode(BTN_A, INPUT);
  pinMode(BTN_B, INPUT);
  pinMode(BTN_C, INPUT);
  pinMode(BTN_D, INPUT);
  pinMode(BTN_E, INPUT);
  pinMode(BTN_F, INPUT);

  Serial.begin(9600);

  display.begin();
  display.setContrast(55);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);

  if (!radio.begin()) {
    display.clearDisplay();
    display.println(F("FAILED TO INITIALIZE RADIO!"));
    display.display();
    while (1); 
  }

  // radio.setPayloadSize(sizeof(buffer));
  radio.openWritingPipe(pipeOut);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening(); 

  display.println(F("READY..."));
  display.display();

  delay(1000);

}

void loop() {
  LoadBufferWithButtonsData(buffer, ++data_id);
  bool success = radio.write(buffer, sizeof(buffer));

  DisplayData(buffer);

  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("- DEBUG MODE -"));
  
  display.print(F("X: ")); display.println(((buffer[6] << 8) | buffer[7]));
  display.print(F("Y: ")); display.println(((buffer[8] << 8) | buffer[9]));
  
  display.print(F("Link: ")); 
  display.println(success ? F("OK") : F("LOST"));

  display.print(F("Btns: "));
  if(buffer[0] == HIGH) display.print(F("A "));
  if(buffer[1] == HIGH) display.print(F("B "));
  if(buffer[2] == HIGH) display.print(F("C "));
  if(buffer[3] == HIGH) display.print(F("D "));
  if(buffer[4] == HIGH) display.print(F("E "));
  if(buffer[5] == HIGH) display.print(F("F "));
  display.display();

  Serial.println("Free RAM: " + String(freeRam()));

  delay(200); 
}
