#include <AtHome.h>

using SoundModule = AtHomeModule<bool, 1>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
NeoPixel led(6);
SoundSensor soundSensor(8);
SoundModule *module = SoundModule::getInstance();
FakeRTC rtc;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    module->setStreams(streams);
    module->setSensor(&soundSensor);
    module->setDisplay(&led);
#ifdef __AVR__
    module->setStorage(&storage);
#endif
    //module->setSerial(0); // Uncomment to reinitialize the id of the module
    module->setClock(&rtc);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}
