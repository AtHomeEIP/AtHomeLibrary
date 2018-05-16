#include <AtHome.h>

using GasModule = AtHomeModule<MQ2GasSensor::Values, 10>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
MQ2GasSensor sensor(8);
GasModule *module = GasModule::getInstance();

const char vendor[] = "AtHome\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
const char serial[] = "XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOXO";

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
    //module->setVendor(vendor); // Set the vendor of the module -> Always an array of 32 bytes
    //module->setSerial(serial); // Set the serial of the module -> Always an array of 32 bytes
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}