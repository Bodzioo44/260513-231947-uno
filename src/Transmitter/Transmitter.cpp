#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>

#include "MPU6050.h"
#include "UTILS.h"
#include "Adafruit_Nokia5110.h"

// Software SPI, Doesnt work with multiple SPI chips
// Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, A2, A3, A4);

// Hardware SPI: D/C A2, CE 8, RST A3
Adafruit_PCD8544 display = Adafruit_PCD8544(A2, 8, A3);


Adafruit_MPU6050 mpu;
QMC5883LCompass compass;
Adafruit_BMP085 bmp;

RF24 radio(9, 10); // CE, CSN
uint8_t buffer[32]; 
const uint8_t addresses[] = { 0xD4, 0xF6 };

int ackDataReceived;

uint8_t data_id = 0;

bool BTN_E_Pressed = false;
bool BTN_F_Pressed = false;
uint8_t E_Value = 0;

void setup() {
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_E, INPUT_PULLUP);
  pinMode(BTN_F, INPUT_PULLUP);

  Serial.begin(9600);

  initializeMPU6050(mpu);
  mpu.setI2CBypass(true);
  initializeQMC5883L(compass);
  initializeBMP180(bmp);

  InitScreen(display);

  if (!radio.begin()) {
    display.clearDisplay();
    display.println(F("FAILED TO INITIALIZE RADIO! CHECK HARDWARE!"));
    display.display();
    while (1); 
  }

  radio.setPALevel(RF24_PA_MIN);
  radio.enableDynamicPayloads();
  radio.enableAckPayload();

  radio.stopListening(); 
  radio.openWritingPipe(addresses[0]);
  // radio.openReadingPipe(1, addresses[1]);


  // radio.setCRCLength(RF24_CRC_16);
  // radio.setDataRate(RF24_250KBPS);


  display.println(F("READY..."));
  display.display();

  delay(1000);

}

void loop() {
  if (WasButtonPressed(BTN_E, BTN_E_Pressed)) {
    E_Value = (E_Value + 1) % 4; // Cycle through 4 states
    Serial.println("BTN_E pressed! New E_Value: " + String(E_Value));
  }

  switch (E_Value) {
    case 0:
      LoadBufferWithButtonsData(buffer, ++data_id);
      DisplayPilotButtons(display, buffer);
      break;
    case 1:
      LoadQMC5883L(compass, buffer);
      LoadMPU6050(mpu, buffer + 9);
      DisplayPilotAccel(display, buffer);
      break;
    case 2:
      LoadQMC5883L(compass, buffer);
      LoadMPU6050(mpu, buffer + 9);
      DisplayPilotGyro(display, buffer);
      break;
    case 3:
      LoadQMC5883L(compass, buffer);
      LoadMPU6050(mpu, buffer + 9);
      LoadBMP180(bmp, buffer + 15);
      printBMP180(bmp);
      DisplayPilotBarometer(display, buffer);
      break;
  }

  bool success = radio.write(buffer, sizeof(buffer));
  if (success) {
      // 4. Check if the receiver hitched data onto the ACK packet
      if (radio.isAckPayloadAvailable()) {
        radio.read(&ackDataReceived, sizeof(ackDataReceived));
        Serial.print("Success! Ack payload received: ");
        Serial.println(ackDataReceived);
      } else {
        Serial.println("Success, but no ACK payload returned.");
      }
    } else {
      Serial.println("Delivery failed (No ACK received at all).");
    }

  DisplayData(buffer);
  Serial.println("Free RAM: " + String(freeRam()));

  delay(200); 
}
