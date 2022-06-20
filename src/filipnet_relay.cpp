#include "filipnet_relay.h"

Relay::Relay(String description, int pin, String mqtt){
      digitalWrite(pin, HIGH);
      pinMode(pin, OUTPUT);
      _description = description;
      _pin = pin;
      _mqtt = mqtt;
}

//void Relay::MQTT(){
//      mqttClient.loop();
//      delay(1);
//}

void Relay::ON(){
      Serial.print("Switch on ");
      Serial.println(_description);
      digitalWrite(_pin, HIGH);
      int pinStatus = digitalRead(_pin);
      Serial.print("Status of GPIO pin ");
      Serial.print(_pin);
      Serial.print(" is ");
      Serial.println(pinStatus);
      //mqttClient.publish(_mqtt+"/response", "on");
      delay(1000);
}

void Relay::OFF(){
      Serial.print("Switch off ");
      Serial.println(_description);
      digitalWrite(_pin, LOW);
      int pinStatus = digitalRead(_pin);
      Serial.print("Status of GPIO pin ");
      Serial.print(_pin);
      Serial.print(" is ");
      Serial.println(pinStatus);
      //mqttClient.publish(_mqtt+"/response", "off");
      delay(1000);
}