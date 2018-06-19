#include <SoftwareSerial.h>
#include <AtHome.h>

using LightModule = AtHomeWiFiModule<uint16_t, 1>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr, nullptr};
GroveChainableLED::Pins grovePins = {7, 8};
GroveChainableLED led(&grovePins);
LightModule *module = AtHomeModule<uint16_t, 1>::getInstance<LightModule>();
ESP8266WiFiCommunicator com(2, 3);
#ifdef __AVR__
SoftwareSerial ATSerial(10, 11);
#else
HardwareSerial &ATSerial = Serial1;
#endif
FakeRTC rtc;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    ATSerial.begin(115200);
    com.setStreamToChipset(&ATSerial);
    com.setWiFiMode(athome::communication::wifi::wifi_mode::STATION);
    GroveDigitalLightSensor *lightSensor = new GroveDigitalLightSensor();
    module->setStreams(streams);
    module->setSensor(lightSensor);
    module->setDisplay(&led);
#ifdef __AVR__
    module->setStorage(&storage);
#endif
    //module->setSerial(0); // Uncomment to reinitialize the id of the module
    module->setClock(&rtc);
    module->setWiFiCommunicator(com);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}