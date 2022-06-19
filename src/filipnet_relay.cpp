#include "filipnet_relay.h"

Relay::Relay(char description, int pin, char mqtt){
    pinMode(pin, OUTPUT);
    _description = description;
    _pin = pin;
    _mqtt = mqtt;
}

void Relay::ON(){
      Serial.print("Switch on ");
      Serial.println(_description);
      digitalWrite(_pin, HIGH);
      int pinStatus = digitalRead(_pin);
      Serial.print("Status of GPIO pin ");
      Serial.print(_pin);
      Serial.print(" is ");
      Serial.println(pinStatus);
      //client.publish(_mqtt+"/response", "on");
      delay(1000);
}

void Relay::OFF(){
      Serial.print("Switch on ");
      Serial.println(_description);
      digitalWrite(_pin, LOW);
      int pinStatus = digitalRead(_pin);
      Serial.print("Status of GPIO pin ");
      Serial.print(_pin);
      Serial.print(" is ");
      Serial.println(pinStatus);
      //client.publish(_mqtt+"/response", "off");
      delay(1000);
}