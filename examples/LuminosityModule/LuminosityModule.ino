#include <AtHome.h>

using LightModule = AtHomeModule<uint16_t, 6>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
GroveChainableLED::Pins grovePins = {7, 8};
GroveChainableLED led(&grovePins);
LightModule *module = LightModule::getInstance();

//const char vendor[] = "AtHome\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
//const char serial[] = "XOXOXOXOXOXOXOXOXOXOXOXOXOXOXOXO";

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
    //module->setVendor(vendor); // Set the vendor of the module -> Always an array of 32 bytes
    //module->setSerial(serial); // Set the serial of the module -> Always an array of 32 bytes
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}