#ifndef FILIPNET_RELAY_H
#define FILIPNET_RELAY_H
#include <Arduino.h>

class Relay {
    private:
        char _description;
        int _pin;
        char _mqtt;
    public:
        Relay(const char description, const int pin, const char mqtt);
        void ON();
        void OFF();
};

#endif