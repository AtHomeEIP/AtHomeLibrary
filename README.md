# WoodBox Development Framework

## Requirements:
- Partial Arduino Core implementing interfaces Print, Printable and Stream -> Used for generic interfaces and avoid duplicates between Arduino and non-Arduino projects
- ArduinoJson library, despite it's name it's fully portable
- Grove' AirQuality library
- Grove' ChainableLED library
- Adafruit NeoPiel library
- TaskScheduler library
- Adafruit DHT universal library

## Compatible environments:

| Environment | Arduino | Energia | mBED                                                            | Bare-Metal |
|-------------|---------|---------|-----------------------------------------------------------------|------------|
| Support     | Y       | Y       | Not yet (not tested and need C++14 support in incoming release) | Not yet    |

## Compatible platforms:

| Component / Platform     | AVR            | ARM                      | MSP430                                                                         | MSP432         | PIC32          |
|--------------------------|----------------|--------------------------|--------------------------------------------------------------------------------|----------------|----------------|
| ESP8266                  | Y (not tested) | Y (not tested)           | N                                                                              | Y              | Y (not tested) |
| Grove Chainable LED      | Y              | Y (not tested)           | Y (not tested, need a small patch but I'm unable to check as I don't have one) | Y (not tested) | Y (not tested) |
| AnalogSensor             | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) |
| Common cathode RGB LED   | Y (not tested) | Y (not tested)           | Y (not tested)                                                                 | Y              | Y (not tested) |
| Grove Air Quality sensor | Y* (not all)   | N                        | N                                                                              | N              | N              |
| NeoPixel                 | Y              | Y* (not all, not tested) | N                                                                              | N              | N              |