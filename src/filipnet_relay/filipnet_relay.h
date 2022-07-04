#ifndef FILIPNET_RELAY_H
#define FILIPNET_RELAY_H
#include <Arduino.h>

class Relay {
    private:
        String _description;
        int _pin;
        String _contact;
        const char* _mqtt;
        // Definition of contact status "Normal Open" (NO)
        int onSignal = LOW;
		int offSignal = HIGH;
    public:
        Relay(String description, const int pin, String contact, const char* mqtt);
        void ON();
        void OFF();
        void INVERT();
        void STATE();
        const char* MQTTTOPIC();
        const char* MQTTSTATE();
};

#endif