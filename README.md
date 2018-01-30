# @@Home Development Kit

This repository holds the source code of the @HOME Development Kit (Framework + examples + PCBs and schematics).

## What is @@Home?

@@Home is a set of smart devices enabling to monitor a house's environment and the possible effects on human health. A smartphone application communicating with sensing devices put into the house, enable the user to get advices, tips and solutions to potential problems, and to communicate with other members of @@Home community, to be able to live better at home.

## Installation and requirements to use this kit:

This development kit requires some external libraries to be present on your environment. If you use Arduino, just download these into your "libraries" folder into your "Arduino" directory.

In the same way, to use this development kit, just download this repository in your "libraries" folder. Once all libraries are downloaded, just open the "Library manager" in the Arduino IDE for it to update your libraries list.

Usual locations of Arduino directory:
- Windows: Documents\Arduino
- Linux: ~/Arduino (ie, in your home folder)
- OS X: ~/Arduino (ie, in your home folder)

Libraries required:
- Partial Arduino Core implementing interfaces Print, Printable and Stream -> Used for generic interfaces and avoid duplicates between Arduino and non-Arduino projects
- ArduinoJson library, despite it's name it's fully portable: https://github.com/bblanchon/ArduinoJson
- Grove LightSensor library: https://github.com/Seeed-Studio/Grove_Digital_Light_Sensor
- Grove AirQuality library: https://github.com/Seeed-Studio/Grove_Air_quality_Sensor
- Grove ChainableLED library: https://github.com/pjpmarques/ChainableLED
- Adafruit NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel
- TaskScheduler library: https://github.com/arkhipenko/TaskScheduler
- Adafruit Sensor library: https://github.com/adafruit/Adafruit_Sensor
- Adafruit TSL2561 library: https://github.com/adafruit/Adafruit_TSL2561

## Compatible environments:

Here is the satatus of compatibility of this development kit with several commonly used environments:

| Environment | Arduino | Energia | Mbed                                                            | Bare-Metal | PlatformIO     |
|-------------|---------|---------|-----------------------------------------------------------------|------------|----------------|
| Support     | Y       | Y       | Not yet (not tested and need C++14 support in incoming release) | Not yet    | Y (not tested) |

## Compatible architectures:

Here is the status of compatibility of components available in this development kit with several common architectures of microcontrollers:

| Component / Platform     | AVR            | ARM                      | MSP430                                                                         | MSP432         | PIC32          | PIC18 | ESP8266 | ESP32 |
|--------------------------|----------------|--------------------------|--------------------------------------------------------------------------------|----------------|----------------|-------|---------|-------|
| AtHome Core             | Y              | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) | ?     | ?       | ?     |
| AtHome WiFi Module      | Y              | Y (not tested)           | N                                                                              | Y              | Y              | ?     | ?       | ?     |
| ESP8266 AT Firmware      | Y (not tested) | Y (not tested)           | N                                                                              | Y              | Y (not tested) | ?     | ?       | ?     |
| Grove Chainable LED      | Y              | Y (not tested)           | Y (not tested, need a small patch but I'm unable to check as I don't have one) | Y (not tested) | Y (not tested) | ?     | ?       | ?     |
| AnalogSensor             | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) | ?     | ?       | ?     |
| ArduinoEEPROM            | Y              | N                        | N                                                                              | N              | N              | ?     | ?       | ?     |
| Common cathode RGB LED   | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) | ?     | ?       | ?     |
| Grove Air Quality sensor | Y (not all)    | N                        | N                                                                              | N              | N              | ?     | ?       | ?     |
| Grove Light Sensor (NI)  | N/A            | N/A                      | N/A                                                                            | N/A            | N/A            | N/A   | N/A     | N/A   |
| MQ2 Gas sensor (NI)      | N/A            | N/A                      | N/A                                                                            | N/A            | N/A            | N/A   | N/A     | N/A   |
| NeoPixel                 | Y              | Y (not all, not tested)  | N                                                                              | N              | N              | ?     | ?       | ?     |
| Blynk (NI)               | N/A            | N/A                      | N/A                                                                            | N/A            | N/A            | N/A   | N/A     | N/A   |

## Low Power Mode

For battery powered modules, here is the list of low power operation mode on several architectures and environment:

| Environment / Architecture | AVR | ARM | MSP430  | MSP432 | PIC32 | PIC18 | ESP8266 | ESP32 |
|----------------------------|-----|-----|---------|--------|-------|-------|---------|-------|
| Arduino (and Energia)      | Y   | N   | Not all | Y      | N     | N     | N       | N     |
| Mbed                       | N/A | Y   | N/A     | N/A    | N/A   | N/A   | N/A     | N/A   |
| Bare-Metal                 | N   | N   | N       | N      | N     | N     | N       | N     |

## Secured communication

Status of secured communication between modules and their server (the @@Home when the set is built by the @@Home team):

| Platform / Environment | Arduino | Mbed | Bare-Metal |
|------------------------|---------|------|------------|
| AVR                    | N       | N/A  | N          |
| ARM                    | N       | N    | N          |
| MSP430                 | N       | N/A  | N          |
| MSP432                 | N       | N/A  | N          |
| PIC32                  | N       | N/A  | N          |
| PIC18                  | N       | N/A  | N          |
| ESP8266                | N       | N/A  | N          |
| ESP32                  | N       | N/A  | N          |
