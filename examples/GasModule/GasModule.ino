#include <AtHome.h>

using GasModule = AtHomeModule<MQ2GasSensor::Values, 15>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
MQ2GasSensor sensor(8);
GasModule *module = GasModule::getInstance();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    module->setSensor(&sensor);
    module->setStreams(streams);
#ifdef __AVR__
    module->setStorage(&storage);
#endif
    module->setCommunicationExecutionInterval(1);
    module->setSensorExecutionInterval(100);
    module->setUploadDataExecutionInterval(1001);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}