#include <AtHome.h>

using GasModule = AtHomeModule<MQ2GasSensor::Values, 1>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
MQ2GasSensor sensor(8);
GasModule *module = GasModule::getInstance();
FakeRTC rtc;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    module->setSensor(&sensor);
    module->setStreams(streams);
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