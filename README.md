# Aquarium

Control of aquarium technology and sensors

## FEATURES
<img src="images/mqtt-dash-example.jpg" alt="Telegram notification example" width="400" align="right" hspace="30" vspace="20"/>
- Connects to the Wireless Access Point / Wifi Router
- Securely connects to the MQTT Broker (TLS)
- Subscribes to the desired MQTT topics
- Handles incoming MQTT messages
- Controls ESP8266 Chip Level Data Pins
- Sends a heartbeat signal to the MQTT broker periodically
- Switches a double relay (230V) for daylight and nightlight
- Monitoring of room temperature, humidity and air pressure with GY-BME280 sensor
- Monitoring water temperature DS18B20 Temperature sensor

<!-- TOC -->

- [Aquarium](#aquarium)
    - [FEATURES](#features)
    - [REQUIREMENTS](#requirements)
        - [LIBRARIES](#libraries)
    - [INSTALL AND USAGE](#install-and-usage)
        - [PINOUT](#pinout)
        - [MQTT TOPICS](#mqtt-topics)
    - [LICENSE](#license)

<!-- /TOC -->

## REQUIREMENTS

- Wemos D1 Mini (should work with other Arduino microcontrollers too)
- PlatformIO or Arduino IDE (for Arduino IDE you have to move files from /src to root-folder)
- MQTT Broker such as the Mosquitto MQTT Broker (https://mosquitto.org/)
- ControllApp such as MQTT Dash (https://play.google.com/store/apps/details?id=net.routix.mqttdash&hl=en)
- 2-Relais Module 5V with Optokoppler low-level trigger compatible with Arduino
- GY-BME280 sensor
- DS18B20 Temperature sensor

### LIBRARIES

- PubSubClient by Nick O'Leary
- esp8266_mdns by dunk
- DallasTemperature by Miles Burton

## INSTALL AND USAGE

### PINOUT

| Sensor-Description | Sensor-PIN | Arduino-PIN | GPIO |
| --- | --- | --- | --- |
| BUILTIN_LED | - | D4 | GPIO 2 |
| DO NOT USE | - | D3 | GPIO 0 | 
| SRD-05VDC-SL-C Relais	| GND | GND | - |
| SRD-05VDC-SL-C Relais	| IN1 | D5 | GPIO 14 |
| SRD-05VDC-SL-C Relais	| IN2 | D6 | GPIO 12 |
| SRD-05VDC-SL-C Relais	| VCC | 5.0 V |	- |
| SRD-05VDC-SL-C Relais	| NC1 + COM1 | Phase L (230V) Nightlight | - |
| SRD-05VDC-SL-C Relais	| NC2 + COM2 | Phase L (230V) Daylight | - |
| GY-BME280 Barometric sensor | VCC | 3V | - |
| GY-BME280 Barometric sensor | GND | GND | - |
| GY-BME280 Barometric sensor | SDA | D2 | GPIO 4 |
| GY-BME280 Barometric sensor | SCL | D1 | GPIO 5 |
| DS18B20 waterproof temperature sensor | red | 5.0V | - |
| DS18B20 waterproof temperature sensor | black | GND | - |
| DS18B20 waterproof temperature sensor | yellow | D7 | GPIO 13 |

* Unlike in my other projects I decided to connect the voltage to the NC pins (normal close) of the double relay. The reason is that in case of an Arduino failure the light should be switched on completely instead of off during a longer vacation.

### MQTT TOPICS

| MQTT Topic | Description | Possible values |
| --- | --- | --- |
| home/outdoor/aquarium/daylight | Switches the daylight on or off | on, off |
| home/outdoor/aquarium/nightlight | Switches the nightlight on or off | on, off |
| home/outdoor/aquarium/temperature | Room temperature | {value} |
| home/outdoor/aquarium/humidity | Room humidity | {value} |
| home/outdoor/aquarium/pressure | Room pressure | {value} |
| home/outdoor/aquarium/watertemperature | Aquarium water temperature | {value} |
| home/outdoor/aquarium/heartbeat | Heartbeat for troubleshooting | on |

## LICENSE

Aquarium and all individual scripts are under the BSD 3-Clause license unless explicitly noted otherwise. Please refer to the LICENSE