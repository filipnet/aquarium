#ifndef FILIPNET_RELAY_H
#define FILIPNET_RELAY_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

class Relay {
    private:
        String _description;
        int _pin;
        String _mqtt;
    public:
        Relay(String description, const int pin, String mqtt);
        void MQTT();
        void ON();
        void OFF();
};

#endif