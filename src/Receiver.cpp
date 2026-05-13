#include <Arduino.h>

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>


#define PWM_left 6
#define PWM_right 5
#define forward_left 4
#define backward_left 3
#define forward_right 7
#define backward_right 8

// const uint64_t pipeOut = 0xE8E8F0F0E1LL;
const uint64_t pipeOut = 0xA4D5C6F7E1LL;
int x = 335;
int y = 335;
RF24 radio (9,10);
int dataToRead[7];

int a,b,c,d;
int oopsie_counter = 0;
int delay_val = 10;
int control_data = -1;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, pipeOut);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(PWM_left, OUTPUT);
  pinMode(PWM_right, OUTPUT);
  pinMode(forward_left, OUTPUT);
  pinMode(backward_left, OUTPUT);
  pinMode(forward_right, OUTPUT);
  pinMode(backward_right, OUTPUT);

  analogWrite(PWM_left, 255);
  analogWrite(PWM_right, 255);
  //digitalWrite(forward_left, HIGH);
  //digitalWrite(forward_right, HIGH);
  //digitalWrite(backward_left, HIGH);
  //digitalWrite(backward_right, HIGH);
}

void loop() {
  x = -1; y = -1, a = -1, b = -1, c= -1, d = -1;
  if (radio.available()) {
    Serial.println("Successfull radio read");
    radio.read(&dataToRead, sizeof(dataToRead));

    x = dataToRead[0]; y = dataToRead[1];
    a = dataToRead[2]; b = dataToRead[3];
    c = dataToRead[4]; d = dataToRead[5];
    control_data = dataToRead[6];

    if (control_data != -1) {
      Serial.println("Control data missmatch!!! abandon ship!!!");
      radio.powerDown(); 
      delay(100); 
      radio.powerUp(); 
      delay(100);

      radio.begin();
      radio.setDataRate(RF24_250KBPS);
      radio.openReadingPipe(0, pipeOut);
      radio.setPALevel(RF24_PA_MIN);
      radio.startListening();

      return;
    }

    Serial.println("Data: ");

    Serial.print("Received: ");
    for (int i = 0; i < 7; i++) {

      Serial.print(dataToRead[i]);
      Serial.print(", ");
    }
    if (a == 0){
      Serial.println("TURNING FRONT");
      digitalWrite(forward_left, HIGH);
      digitalWrite(forward_right, HIGH);
      digitalWrite(backward_left, LOW);
      digitalWrite(backward_right, LOW);
    }
    else if (c == 0){
      Serial.println("TURNING BACK");
      digitalWrite(forward_left, LOW);
      digitalWrite(forward_right, LOW);
      digitalWrite(backward_left, HIGH);
      digitalWrite(backward_right, HIGH);
    }
    else if (d == 0){
      Serial.println("TURNING LEFT");
      digitalWrite(forward_left, LOW);
      digitalWrite(forward_right, HIGH);
      digitalWrite(backward_left, HIGH);
      digitalWrite(backward_right, LOW);    
    }
    else if (b == 0){
      Serial.println("TURNING RIGHT");
      digitalWrite(forward_left, HIGH);
      digitalWrite(forward_right, LOW);
      digitalWrite(backward_left, LOW);
      digitalWrite(backward_right, HIGH); 
    }
    else{
      Serial.println("TURNING OFF");
      digitalWrite(forward_left, LOW);
      digitalWrite(forward_right, LOW);
      digitalWrite(backward_left, LOW);
      digitalWrite(backward_right, LOW); 
    }
    oopsie_counter = 0;
  }
  else {
    oopsie_counter++;
    if (oopsie_counter == 100) {
      Serial.println("TURNING OFF");
      Serial.println("critical failure, radio absent for 1000ms");
      digitalWrite(forward_left, LOW);
      digitalWrite(forward_right, LOW);
      digitalWrite(backward_left, LOW);
      digitalWrite(backward_right, LOW); 
      oopsie_counter = 0;
      
    }
    delay(delay_val);
  }
}
