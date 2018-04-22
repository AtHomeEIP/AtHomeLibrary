#include <AtHome.h>

using SoundModule = AtHomeModule<bool, 15>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
NeoPixel led(6);
SoundSensor soundSensor(8);
SoundModule *module = SoundModule::getInstance();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    module->setStreams(streams);
    module->setSensor(&soundSensor);
    module->setDisplay(&led);
    module->setCommunicationExecutionInterval(10);
    module->setSensorExecutionInterval(100);
    module->setUploadDataExecutionInterval(1500);
#ifdef __AVR__
    module->setStorage(&storage);
#endif
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}
