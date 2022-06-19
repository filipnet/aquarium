#ifndef FILIPNET_RELAY_H
#define FILIPNET_RELAY_H
#include <Arduino.h>

class Relay {
    private:
        String _description;
        int _pin;
        String _mqtt;
    public:
        Relay(String description, const int pin, String mqtt);
        void ON();
        void OFF();
};

#endif