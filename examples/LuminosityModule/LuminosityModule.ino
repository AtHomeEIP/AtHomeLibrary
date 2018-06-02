#include <AtHome.h>

using LightModule = AtHomeModule<uint16_t, 1>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
GroveChainableLED::Pins grovePins = {7, 8};
GroveChainableLED led(&grovePins);
LightModule *module = LightModule::getInstance();
FakeRTC rtc;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    GroveDigitalLightSensor *lightSensor = new GroveDigitalLightSensor();
    module->setStreams(streams);
    module->setSensor(lightSensor);
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