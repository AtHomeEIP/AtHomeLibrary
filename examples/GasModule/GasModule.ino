#include <AtHome.h>

using GasModule = AtHomeModule<MQ2GasSensor::Values, 15>;

ArduinoEEPROM storage;
Stream *streams[] = {&Serial, nullptr};
MQ2GasSensor sensor(8);
GasModule *module = GasModule::getInstance();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    module->setSensor(&sensor);
    module->setStreams(streams);
    module->setStorage(&storage);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}