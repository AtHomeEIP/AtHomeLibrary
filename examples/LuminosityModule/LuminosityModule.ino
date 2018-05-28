#include <AtHome.h>

using LightModule = AtHomeModule<uint16_t, 6>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
GroveChainableLED::Pins grovePins = {7, 8};
GroveChainableLED led(&grovePins);
LightModule *module = LightModule::getInstance();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    GroveDigitalLightSensor *lightSensor = new GroveDigitalLightSensor();
    module->setStreams(streams);
    module->setSensor(lightSensor);
    module->setDisplay(&led);
    module->setCommunicationExecutionInterval(10);
    module->setSensorExecutionInterval(1000);
    module->setUploadDataExecutionInterval(5000);
#ifdef __AVR__
    module->setStorage(&storage);
#endif
    //module->setSerial(42);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}