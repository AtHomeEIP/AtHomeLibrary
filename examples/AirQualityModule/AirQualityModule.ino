#include <AtHome.h>

using AirQualityModule = AtHomeModule<uint8_t, 1>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
GroveAirQualitySensor airSensor(A0);
AirQualityModule *module = AirQualityModule::getInstance();
FakeRTC rtc;
//NeoPixel led(3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  module->setSensor(&airSensor);
  module->setStreams(streams);
  //module->setDisplay(&led);
  module->setClock(&rtc);
  #ifdef __AVR__
  module->setStorage(&storage);
  #endif
  //module->setSerial(0); // Uncomment to reinitialize the id of the module
  module->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}
