#include <Arduino.h>
#include <ESP8266WiFi.h>
#define MQTT_MAX_PACKET_SIZE 256
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <Adafruit_BME280.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "filipnet_relay.h"
#include "credentials.h"
#include "config.h"

const char *hostname = WIFI_HOSTNAME;
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *mqttServer = MQTT_SERVER;
const int mqttPort = MQTT_PORT;
const char *mqttUser = MQTT_USERNAME;
const char *mqttPassword = MQTT_PASSWORD;
const char *mqttID = MQTT_ID;

unsigned long sensor_previousMillis = 0;
const long sensor_interval = SENSOR_INTERVALL;
unsigned long heartbeat_previousMillis = 0;
const long heartbeat_interval = HEARTBEAT_INTERVALL;
float temperature_local;
float humidity_local;
float pressure_local;
float watertemperature_local;

// Data wire is plugged into port D7 on the Arduino
OneWire oneWire(WATERTEMP_SENSOR);                   // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);                 // Pass our oneWire reference to Dallas Temperature.
DeviceAddress insideThermometer, outsideThermometer; // arrays to hold device addresses
Adafruit_BME280 bme;                                 // Declaration of BME280 sensor

WiFiClientSecure espClient;
PubSubClient client(espClient);

// Create relay instances by library filipnet_relay
Relay Daylight("Aquarium Daylight", RELAY_DAYLIGHT, "home/indoor/aquarium/daylight");
Relay Nightlight("Aquarium Nightlight", RELAY_NIGHTLIGHT, "home/indoor/aquarium/nightlight");
Relay Airpump("Aquarium Airpump", RELAY_AIRPUMP, "home/indoor/aquarium/airpump");
Relay Filter("Aquarium Filter", RELAY_FILTER, "home/indoor/aquarium/filter");

void setRelayStatus(char *topic, byte *payload, unsigned int length)
{
  String mqttTopic = String(topic);
  String mqttPayload;
  for (unsigned int i = 0; i < length; i++)
  {
    mqttPayload += (char)payload[i];
  }
  String pinStatus;

  if (mqttTopic == "home/indoor/aquarium/nightlight")
  {
    if (mqttPayload == "on") { Nightlight.ON(); }
    else if (mqttPayload == "off") { Nightlight.OFF(); }
    else { Serial.println("No valid mqtt command"); }
  }

  else if (mqttTopic == "home/indoor/aquarium/daylight")
  {
    if (mqttPayload == "on") { Daylight.ON(); }
    else if (mqttPayload == "off") { Daylight.OFF(); }
    else { Serial.println("No valid mqtt command"); }
  }

  else if (mqttTopic == "home/indoor/aquarium/airpump")
  {
    if (mqttPayload == "on") { Airpump.ON(); }
    else if (mqttPayload == "off") { Airpump.OFF(); }
    else { Serial.println("No valid mqtt command"); }
  }

  else if (mqttTopic == "home/indoor/aquarium/filter")
  {
    if (mqttPayload == "on") { Filter.ON(); }
    else if (mqttPayload == "off") { Filter.OFF(); }
    else { Serial.println("No valid mqtt command"); }
  }
  Serial.println("");
}

// Pointer to a message callback function called when a message arrives for a subscription created by this client.
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message topic: ");
  Serial.print(topic);
  Serial.print(" | Message Payload: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  setRelayStatus(topic, payload, length);
}

void reconnect()
{
  while (!client.connected())
  {
    WiFi.mode(WIFI_STA);
    WiFi.hostname(hostname);
    delay(100);
    Serial.println();
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to WiFi network");
    Serial.print("  SSID: ");
    Serial.print(ssid);
    Serial.print(" / Channel: ");
    Serial.println(WiFi.channel());
    Serial.print("  IP Address: ");
    Serial.print(WiFi.localIP());
    Serial.print(" / Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("  Gateway: ");
    Serial.print(WiFi.gatewayIP());
    Serial.print(" / DNS: ");
    Serial.print(WiFi.dnsIP());
    Serial.print(", ");
    Serial.println(WiFi.dnsIP(1));
    Serial.println("");

    // https://pubsubclient.knolleary.net/api.html
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);
    Serial.println("Connecting to MQTT broker");
    Serial.print("  MQTT Server: ");
    Serial.println(mqttServer);
    Serial.print("  MQTT Port: ");
    Serial.println(mqttPort);
    Serial.print("  MQTT Username: ");
    Serial.println(mqttUser);
    Serial.print("  MQTT Identifier: ");
    Serial.println(mqttID);
    Serial.println("");

    while (!client.connected())
    {
      if (client.connect(mqttID, mqttUser, mqttPassword))
      {
        Serial.println("Connected to MQTT broker");
        Serial.println("Subscribe MQTT Topics");
        // Subscribe the following mqtt topics
        client.subscribe("home/indoor/aquarium/daylight");
        client.subscribe("home/indoor/aquarium/nightlight");
        client.subscribe("home/indoor/aquarium/airpump");
        client.subscribe("home/indoor/aquarium/filter");
        Serial.println("");
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        Serial.print("Connection to MQTT broker failed with state: ");
        Serial.println(client.state());
        char puffer[100];
        espClient.getLastSSLError(puffer, sizeof(puffer));
        Serial.print("TLS connection failed with state: ");
        Serial.println(puffer);
        Serial.println("");
        //digitalWrite(relayNightlight, HIGH);
        delay(4000);
      }
    }
  }
}

// Function to receive MQTT messages
void mqttloop()
{
  if (!client.loop())
    client.connect(mqttID);
}

// Function to send MQTT messages
void mqttsend(const char *_topic, const char *_data)
{
  client.publish(_topic, _data);
}

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

void I2CAddressFinder()
{
  Serial.println();
  Serial.println("I2C scanner. Scanning ...");
  byte count = 0;
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Wire.begin();
  for (byte i = 1; i < 120; i++)
  {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(1);
    }
  }
  Serial.print("Found ");
  Serial.print(count, DEC);
  Serial.println(" device(s).");
  Serial.println("");
}

void initialize_bme280()
{
  Serial.print("Initialize BME280 module: ");
  if (!bme.begin(I2C_ADDRESS_BME280))
  {
    Serial.println(F("BME280 allocation failed"));
  }
  else
  {
    Serial.println("OK");
  }
}

void readsensor_bme280()
{
  temperature_local = bme.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature_local);
  Serial.println(" *C");
  static char temperature_local_char[7];
  dtostrf(temperature_local, 1, 2, temperature_local_char);
  Serial.print("  MQTT publish home/indoor/aquarium/temperature: ");
  Serial.println(temperature_local_char);
  client.publish("home/indoor/aquarium/temperature", temperature_local_char, true);
  delay(100);

  humidity_local = bme.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity_local);
  Serial.println(" %");
  static char humidity_local_char[7];
  dtostrf(humidity_local, 1, 2, humidity_local_char);
  Serial.print("  MQTT publish home/indoor/aquarium/humidity: ");
  Serial.println(humidity_local_char);
  client.publish("home/indoor/aquarium/humidity", humidity_local_char, true);
  delay(100);

  pressure_local = (bme.readPressure() / 100.0F);
  Serial.print("Pressure: ");
  Serial.print(pressure_local);
  Serial.println(" hPa");
  static char pressure_local_char[7];
  dtostrf(pressure_local, 1, 2, pressure_local_char);
  Serial.print("  MQTT publish home/indoor/aquarium/pressure: ");
  Serial.println(pressure_local_char);
  client.publish("home/indoor/aquarium/pressure", pressure_local_char, true); // Pressure (hPa)
  delay(100);
}

void readsensor_ds18b20()
{
  sensors.requestTemperatures();
  watertemperature_local = sensors.getTempCByIndex(0);
  Serial.print("Water temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" *C");
  static char watertemperature_local_char[7];
  dtostrf(watertemperature_local, 1, 2, watertemperature_local_char);
  Serial.print("  MQTT publish home/indoor/aquarium/watertemperature: ");
  Serial.println(watertemperature_local_char);
  client.publish("home/indoor/aquarium/watertemperature", watertemperature_local_char, true);
  delay(100);
}

void sensor()
{
  unsigned long sensor_currentMillis = millis();
  if (sensor_currentMillis - sensor_previousMillis >= sensor_interval)
  {
    sensor_previousMillis = sensor_currentMillis;
    readsensor_bme280();
    readsensor_ds18b20();
    Serial.println("");
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  pinMode(LED_BUILTIN, OUTPUT);
  // Turn off relay-ports on startup
  //digitalWrite(relayNightlight, HIGH);
  //pinMode(relayNightlight, OUTPUT);
  //digitalWrite(relayDaylight, HIGH);
  //pinMode(relayDaylight, OUTPUT);
  ///digitalWrite(relayAirpump, HIGH);
  //pinMode(relayAirpump, OUTPUT);
  //digitalWrite(relayFilter, HIGH);
  //pinMode(relayFilter, OUTPUT);
  espClient.setInsecure();
  I2CAddressFinder();
  initialize_bme280();
  sensors.begin(); // DS18B20
  reconnect();
}

void loop()
{
  client.loop();
  reconnect();
  sensor();
  heartbeat();
  mqttloop();
}
