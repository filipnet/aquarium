#include "heartbeat.h"
#include <ESP8266WiFi.h>
#define MQTT_MAX_PACKET_SIZE 256
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include "config.h"

unsigned long heartbeat_previousMillis = 0;
const long heartbeat_interval = HEARTBEAT_INTERVALL;

void heartbeat()
{
  unsigned long heartbeat_currentMillis = millis();
  if (heartbeat_currentMillis - heartbeat_previousMillis >= heartbeat_interval)
  {
    heartbeat_previousMillis = heartbeat_currentMillis;
    Serial.println("Send heartbeat signal to MQTT broker");
    Serial.println("");
    client.publish("home/indoor/aquarium/heartbeat", "on");
  }
}