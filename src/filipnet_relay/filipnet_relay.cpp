#include "filipnet_relay.h"

Relay::Relay(String description, int pin, String contact, const char* mqtt){
      digitalWrite(pin, HIGH);
      pinMode(pin, OUTPUT);
      _description = description;
      _pin = pin;
      _contact = contact;
      _mqtt = mqtt;
      if (_contact == "NC") { INVERT(); } // Invert HIGH and LOW if contact status is "Normal Close" (NC)
}

void Relay::ON(){
      Serial.print("Switch on ");
      Serial.println(_description);
      digitalWrite(_pin, onSignal);
      Relay::STATE();
}

void Relay::OFF(){
      Serial.print("Switch off ");
      Serial.println(_description);
      digitalWrite(_pin, offSignal);
      Relay::STATE();
}

void Relay::INVERT() {
      const int tempSignal = this->onSignal;
      this->onSignal = this->offSignal;
      this->offSignal = tempSignal;
}

void Relay::STATE(){
      int pinStatus = digitalRead(_pin);
      Serial.print("Status of GPIO pin ");
      Serial.print(_pin);
      Serial.print(" is ");
      Serial.println(pinStatus);
      delay(1000);
}

const char* Relay::MQTTTOPIC(){
      String mqttTopic = _mqtt;
      mqttTopic.concat("/response");
      const char * mqttTopicResponse = mqttTopic.c_str();
      return mqttTopicResponse;
}

const char* Relay::MQTTSTATE(){
      int pinStatus = digitalRead(_pin);
      char const* mqttState = pinStatus.c_str();
      return mqttState;
}