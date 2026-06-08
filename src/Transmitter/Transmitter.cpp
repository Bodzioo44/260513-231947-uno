#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>

// #include "MPU6050.h"
#include "MyCompass.h"
#include "MyMPU6050.h"
#include "MyBMP180.h"
#include "MyDisplay.h"

#include "UTILS.h"
// #include "Adafruit_Nokia5110.h"
// #include "Gavin_Nokia5110.h"

// Software SPI, Doesnt work with multiple SPI chips???
// Adafruit_PCD8544 display = Adafruit_PCD8544(13, 11, A2, A3, A4);

// Hardware SPI: D/C A2, CE 8, RST A3
// Adafruit_PCD8544 display = Adafruit_PCD8544(DC, CE, RST);
// NOKIA5110_TEXT display(RST, CE, DC);


// Adafruit_MPU6050 mpu;
// QMC5883LCompass compass;
// Adafruit_BMP085 bmp;

MyDisplay display;

MyMPU6050 mpu;
MyCompass compass;
MyBMP180 bmp;

RF24 radio(9, 10); // CE, CSN
uint8_t buffer[32]; 
const uint8_t addresses[] = { 0xD4, 0xF6 };

Header header;
ButtonsData btns_data;
uint8_t transmissionID = 0;

bool BTN_E_Pressed = false;
bool BTN_F_Pressed = false;
uint8_t E = 0;
uint8_t F = 0;
bool TX_screen = true;

void setup() {
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_E, INPUT_PULLUP);
  pinMode(BTN_F, INPUT_PULLUP);

  Serial.begin(115200);

  display.initialize();

  mpu.initialize(); // has to go first, contains I2C bypass
  compass.initialize();
  bmp.initialize();
  // initializeQMC5883L(compass);
  // initializeBMP180(bmp);


  if (!radio.begin()) {
    display.message(F("FAILED TO INITIALIZE RADIO! CHECK HARDWARE!"));
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

  display.message(F("Transmitter Initialized!"));
  delay(1000);

  header.DataType = DATA_TYPE::BUTTONS_DATA_TX;
  header.RequestedData = DATA_TYPE::NONE;
  header.TransmissionID = transmissionID;

}

void loop() {
  if (WasButtonPressed(BTN_E, BTN_E_Pressed)) {
    E = (E + 1) % 5;
    TX_screen = true;
  }
  if (WasButtonPressed(BTN_F, BTN_F_Pressed)) {
    F = (F + 1) % 4;
    TX_screen = false;
  }

  btns_data = ReadButtons();

  // Transmitter side
  if (TX_screen) {
    header.RequestedData = DATA_TYPE::NONE;
    switch (E) {
      case 0: {
        CompassData data = compass.readCompassFromSensor();
        display.displayMag(data, TX_screen);
        break;
      }
      case 1: {
        MPU6050Data data = mpu.readMPU6050FromSensor();
        display.displayAccel(data, TX_screen);
        break;
      }
      case 2: {
        MPU6050Data data = mpu.readMPU6050FromSensor();
        display.displayGyro(data, TX_screen);
        break;
      }
      case 3: {
        BMP180Data data = bmp.readBMP180FromSensor();
        display.displayBaro(data, TX_screen);
        break;
      }
      case 4: {
        display.displayButtons(btns_data);
        break;
      }
      default:
        break;
    }
  }
  // Receiver side
  else {
    switch (F) {
      case 0: {
        header.RequestedData = DATA_TYPE::QMC5883L_DATA_RX;
        CompassData data = compass.readCompassFromBuffer(buffer);
        display.displayMag(data, TX_screen);
        break;
      }
      case 1: {
        header.RequestedData = DATA_TYPE::MPU6050_DATA_RX;
        MPU6050Data data = mpu.readMPU6050FromBuffer(buffer);
        display.displayAccel(data, TX_screen);
        break;
      }
      case 2: {
        header.RequestedData = DATA_TYPE::MPU6050_DATA_RX;
        MPU6050Data data = mpu.readMPU6050FromBuffer(buffer);
        display.displayGyro(data, TX_screen);
        break;
      }
      case 3: {
        header.RequestedData = DATA_TYPE::BMP180_DATA_RX;
        BMP180Data data = bmp.readBMP180FromBuffer(buffer);
        display.displayBaro(data, TX_screen);
        break;
      }
      default: 
        break;
    }
  }

  // Loads buttons into buffer, alongside request for new data
  header.DataType = DATA_TYPE::BUTTONS_DATA_TX;
  header.TransmissionID = transmissionID;
  LoadHeader(buffer, header);
  LoadBufferWithButtonsData(buffer, btns_data);

  Serial.println(F("Sending data to RX..."));
  DisplayData(buffer);
  bool success = radio.write(buffer, sizeof(buffer));

  // Saves AckPayload into buffer
  if (success) {
    transmissionID++;
    if (radio.isAckPayloadAvailable()) {
      radio.read(&buffer, sizeof(buffer));
      header = ReadHeader(buffer);
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

  // delay(200); 

}
