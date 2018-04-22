#include <AtHome.h>

using LightModule = AtHomeModule<uint16_t, 15>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
GroveChainableLED::Pins grovePins = {7, 8};
GroveChainableLED led(&grovePins);
GroveDigitalLightSensor *lightSensor = GroveDigitalLightSensor::getInstance();
LightModule *module = LightModule::getInstance();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    module->setStreams(streams);
    module->setSensor(lightSensor);
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