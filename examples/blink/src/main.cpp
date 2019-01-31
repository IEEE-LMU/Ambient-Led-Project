#include <Arduino.h>
#define LED_PIN 2

void setup() {
  pinMode(LED_PIN, OUTPUT);

  //pinMode(0, INPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);

  /*if(digitalRead(0) == 0){
    digitalWrite(LED_PIN,HIGH);
  }else{
    digitalWrite(LED_PIN,LOW);
  }*/
}