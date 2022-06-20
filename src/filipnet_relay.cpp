#include "filipnet_relay.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

/* Troubleshooting custom class inheriting from PubSubClient
https://hobbytronics.com.pk/arduino-custom-library-and-pubsubclient-call-back/
https://github.com/knolleary/pubsubclient/issues/300
https://github.com/xluthi/pubsubclient/blob/master/examples/mqtt_inheritance/mqtt_inheritance.ino
*/

Relay::Relay(String description, int pin, String mqtt){
      //MQTTClient.setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
      digitalWrite(pin, HIGH);
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
      //MQTTClient.publish(_mqtt+"/response", "on");
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
      //MQTTClient.publish(_mqtt+"/response", "off");
      delay(1000);
}