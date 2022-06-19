#ifndef FILIPNET_RELAY_H
#define FILIPNET_RELAY_H
#include <Arduino.h>

class Relay {
    private:
        int _pin;
        char _mqtt;
    public:
        Relay(int pin, char mqtt);
        void ON();
        void OFF();
};

#endif