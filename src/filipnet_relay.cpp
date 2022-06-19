#include "filipnet_relay.h"

Relay::Relay(const int pin, char mqtt){
    pinMode(pin, OUTPUT);
    _pin = pin;
    _mqtt = mqtt;
}

void Relay::ON(){
    digitalWrite(_pin, HIGH);
}

void Relay::OFF(){
    digitalWrite(_pin, LOW);
}