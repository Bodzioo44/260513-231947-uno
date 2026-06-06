#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>

#include "MPU6050.h"
#include "UTILS.h"
#include "Adafruit_Nokia5110.h"
// #include "Gavin_Nokia5110.h"

// Software SPI, Doesnt work with multiple SPI chips???
// Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, A2, A3, A4);

// Hardware SPI: D/C A2, CE 8, RST A3
Adafruit_PCD8544 display = Adafruit_PCD8544(DC, CE, RST);
// NOKIA5110_TEXT display(RST, CE, DC);


Adafruit_MPU6050 mpu;
QMC5883LCompass compass;
Adafruit_BMP085 bmp;

RF24 radio(9, 10); // CE, CSN
uint8_t buffer[32]; 
const uint8_t addresses[] = { 0xD4, 0xF6 };

Header header;
ButtonsData btns_data;
uint8_t transmissionID = 0;

bool BTN_E_Pressed = false;
bool BTN_F_Pressed = false;
uint8_t current_screen = 0;

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
  radio.openReadingPipe(1, addresses[1]);


  // radio.setCRCLength(RF24_CRC_16);
  // radio.setDataRate(RF24_250KBPS);

  display.println(F("READY..."));
  display.display();

  delay(1000);

  header.DataType = DATA_TYPE::BUTTONS_DATA_TX;
  header.RequestedData = DATA_TYPE::NONE;
  header.TransmissionID = transmissionID;

}

void loop() {
  if (WasButtonPressed(BTN_E, BTN_E_Pressed)) {
    current_screen = (current_screen + 1) % 4;
  }
  if (WasButtonPressed(BTN_F, BTN_F_Pressed)) {
    if (current_screen == 0) { current_screen = 4; }
    else { current_screen -=1; } 
  }

  btns_data = ReadButtons();
  // This displays previous data, and requests a new one.
  // based on currently selected screen
  // TODO: Change this switch case to match received header data type?
  // Buttons only change requested data type?
  // If none were selected (or connection was lost)it defaults to debug screen?
  // BAD IDEA, we want to display data from TX too, so it cant be based on received data
  switch (current_screen) {
    // Display Button Screen
    case 0:
      header.RequestedData = DATA_TYPE::NONE;
      DisplayButtons(display, btns_data);
      break;
    // Display Accel Screen
    case 1: {
      MPU6050Data data = ReadMPU6050FromBuffer(buffer+3);
      DisplayAccel(display, data, header);
      header.RequestedData = DATA_TYPE::MPU6050_DATA_RX;
      break;
    }
    // Display Gyro Screen
    case 2: {
      MPU6050Data data = ReadMPU6050FromBuffer(buffer+3);
      DisplayGyro(display, data, header);
      header.RequestedData = DATA_TYPE::MPU6050_DATA_RX;
      break;
    }
    // Display Baro Screen
    case 3: {
      BMP180Data data = ReadBMP180FromBuffer(buffer+3);
      DisplayBaro(display, data, header);
      header.RequestedData = DATA_TYPE::BMP180_DATA_RX;
      break;
    }
    // Display Compass Screen
    case 4: {
      QMCL588LData data = ReadQMC5883LFromBuffer(buffer+3);
      DisplayMag(display, data, header);
      header.RequestedData = DATA_TYPE::QMC5883L_DATA_RX;
      break;
    }
  }

  // Loads buttons into buffer, alongside request for new data
  header.DataType = DATA_TYPE::BUTTONS_DATA_TX;
  header.TransmissionID = transmissionID;
  LoadHeader(buffer, header);
  LoadBufferWithButtonsData(buffer+3, btns_data);

  bool success = radio.write(buffer, sizeof(buffer));

  // Saves AckPayload into buffer
  if (success) {
    if (radio.isAckPayloadAvailable()) {
      radio.read(&buffer, sizeof(buffer));
        Serial.println(F("Success! Ack payload received: "));
        DisplayData(buffer);
    }
    else {
      Serial.println(F("Success, but no ACK payload returned."));
    }
  } 
  else {
    Serial.println(F("Delivery failed (No ACK received at all)."));
  }

  Serial.print(F("Free RAM: ")); Serial.println(freeRam());

  delay(200); 
  transmissionID++;
}
