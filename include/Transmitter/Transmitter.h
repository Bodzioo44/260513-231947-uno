#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Wire.h>

#include "MyCompass.h"
#include "MyMPU6050.h"
#include "MyBMP180.h"
#include "MyDisplay.h"
#include "UTILS.h"

#define JOY_X A0
#define JOY_Y A1
#define BTN_A 2
#define BTN_B 3
#define BTN_C 4
#define BTN_D 5
#define BTN_E 6
#define BTN_F 7

void setup();
void loop();



ButtonsData ReadButtons();
bool WasButtonPressed(int Button, bool& wasPressed);

#endif