#include <ArduinoUnit.h>
#include <AtHome.h>
#include "TestARGBLed.hpp"

void setup() {
    Serial.begin(9600);
    while (!Serial);
    delay(5000);
    Serial.println(F("Feature: As an AtHome module I want to control a display in order to transmit an information"));
}

void loop() {
    Test::run();
}
