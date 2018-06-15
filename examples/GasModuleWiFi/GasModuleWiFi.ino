#include <AtHome.h>

using GasModule = AtHomeWiFiModule<MQ2GasSensor::Values, 1>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
NativeArduinoESPWiFiCommunicator com;
Stream *streams[] = {&Serial, nullptr, nullptr};
MQ2GasSensor sensor(8);
GasModule *module = AtHomeModule<MQ2GasSensor::Values, 1>::getInstance<GasModule>();
FakeRTC rtc;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    module->setSensor(&sensor);
    module->setStreams(streams);
#ifdef __AVR__
    module->setStorage(&storage);
#endif
    module->setSerial(0); // Uncomment to reinitialize the id of the module
    module->setClock(&rtc);
    module->setWiFiCommunicator(com);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}
