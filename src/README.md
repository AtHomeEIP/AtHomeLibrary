# AtHome Development Kit

This repository holds the source code of the AtHome Development Kit (Framework + examples + PCBs and schematics).

Development documentation is available at https://woodbox.gitlab.io/Framework

## What is AtHome?

AtHome is a set of smart devices enabling to monitor a house's environment and the possible effects on human health.
A smartphone application communicating with sensing devices put into the house, enable the user to get advices, tips and solutions to potential problems,
and to communicate with other members of AtHome community, to be able to live better at home.

## Installation and requirements to use this kit:

This development kit requires some external libraries to be present on your environment. If you use Arduino, just download these into your "libraries" folder into your "Arduino" directory.

In the same way, to use this development kit, just download this repository in your "libraries" folder. Once all libraries are downloaded, just open the "Library manager" in the Arduino IDE for it to update your libraries list.

Usual locations of Arduino directory:
- Windows: Documents\Arduino
- Linux: ~/Arduino (ie, in your home folder)
- OS X: ~/Arduino (ie, in your home folder)

Required libraries:
- Partial Arduino Core implementing interfaces Print, Printable and Stream -> Used for generic interfaces and avoid duplicates between Arduino and non-Arduino projects
- TaskScheduler library: https://github.com/arkhipenko/TaskScheduler

Optional libraries:
- Grove LightSensor library: https://github.com/Seeed-Studio/Grove_Digital_Light_Sensor (if using Grove Digital Light Sensor)
- Grove ChainableLED library: https://github.com/Seeed-Studio/Grove_Chainable_RGB_LED (if using Grove Chainable LEDs)
- Adafruit NeoPixel library: https://github.com/adafruit/Adafruit_NeoPixel (if using NeoPixel)

Recommended microcontroler platform:
- MCU compatible with Arduino core (either 8/16/32bits)
- C++11 compiler support
- Internal persistent storage (FRAM is better, EEPROM other way)
- Internal RTC
- At least 32KB of program memory (Warning: not counting bootloader size, some chips such as Microchip PIC32MX or Espressif ESP8265/8266/32 will require much more program memory)
- At least 2KB of RAM

Requirements can be reduced depending on components used and by tweaking the framework configuration. See AtHomeConfig.h to disable unused features.

## Compatible environments:

Here is the satatus of compatibility of this development kit with several commonly used environments:

| Environment | Arduino | Energia | Mbed                                                            | Bare-Metal | PlatformIO     |
|-------------|---------|---------|-----------------------------------------------------------------|------------|----------------|
| Support     | Y       | Y       | Not yet (not tested and need C++14 support in incoming release) | Not yet    | Y (not tested) |

## Compatible architectures:

Here is the status of compatibility of components available in this development kit with several common architectures of microcontrollers:

| Component / Platform     | AVR            | ARM                      | MSP430                                                                         | MSP432         | PIC32          | PIC18 | ESP8266 | ESP32 |
|--------------------------|----------------|--------------------------|--------------------------------------------------------------------------------|----------------|----------------|-------|---------|-------|
| AtHome Core              | Y              | Y (not tested)           | Y                                                                              | Y              | Y (not tested) | ?     | Y       | ?     |
| AtHome WiFi Module       | Y              | Y (not tested)           | N                                                                              | Y              | Y              | ?     | ?       | ?     |
| ESP8266 AT Firmware      | Y (not tested) | Y (not tested)           | N                                                                              | Y              | Y (not tested) | ?     | N/A     | ?     |
| Grove Chainable LED      | Y              | Y (not tested)           | Y (not tested, need a small patch but I'm unable to check as I don't have one) | Y (not tested) | Y (not tested) | ?     | ?       | ?     |
| AnalogSensor             | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) | ?     | Y       | ?     |
| ArduinoEEPROM            | Y              | N                        | N                                                                              | N              | N              | ?     | ?       | ?     |
| Common cathode RGB LED   | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) | ?     | Y       | ?     |
| Common anode RGB LED     | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y (not tested) | Y (not tested) | ?     | Y       | ?     |
| Grove Air Quality sensor | N              | N                        | N                                                                              | N              | ?              | ?     | ?       | ?     |
| Grove Light Sensor       | Y              | Y                        | N                                                                              | N              | ?              | ?     | ?       | ?     |
| MQ2 Gas sensor           | Y              | Y                        | Y                                                                              | Y              | ?              | ?     | ?       | ?     |
| NeoPixel                 | Y              | Y (not all, not tested)  | N                                                                              | N              | N              | ?     | ?       | ?     |
| Binary Sound Sensor      | Y              | Y                        | Y                                                                              | Y              | Y              | ?     | ?       | ?     |
| Dummy Sensor             | Y              | Y                        | Y                                                                              | Y              | Y              | ?     | ?       | ?     |
| Monochromatic LED        | Y              | Y                        | Y                                                                              | Y              | Y              | ?     | ?       | ?     |
| PWM LED                  | Y              | Y                        | Y                                                                              | Y              | Y              | ?     | ?       | ?     |
| Thermistor               | Y              | Y                        | Y                                                                              | Y              | Y              | ?     | ?       | ?     |
| MSP430FRAM               | N              | N                        | Y                                                                              | N              | N              | N     | N       | N     |
| Blynk (NI)               | N/A            | N/A                      | N/A                                                                            | N/A            | N/A            | N/A   | N/A     | N/A   |
| Photoresistor (Not ready)| Y              | Y                        | Y                                                                              | Y              | Y              | ?     | ?       | ?     |
| FakeRTC                  | Y              | Y                        | Y                                                                              | Y              | Y              | N     | ?       | ?     |
| DS1307 (Not ready)       | Y              | ?                        | ?                                                                              | ?              | ?              | ?     | ?       | ?     |

## Low Power Mode

For battery powered modules, here is the list of low power operation mode on several architectures and environment:

| Environment / Architecture | AVR | ARM | MSP430  | MSP432 | PIC32 | PIC18 | ESP8266 | ESP32 |
|----------------------------|-----|-----|---------|--------|-------|-------|---------|-------|
| Arduino (and Energia)      | Y   | N   | Not all | Y      | N     | N     | Y       | N     |
| Mbed                       | N/A | Y   | N/A     | N/A    | N/A   | N/A   | N/A     | N/A   |
| Bare-Metal                 | N   | N   | N       | N      | N     | N     | N       | N     |

## Secured communication [WIP]

Status of secured communication between modules and their server (the AtHome when the set is built by the AtHome team):

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
