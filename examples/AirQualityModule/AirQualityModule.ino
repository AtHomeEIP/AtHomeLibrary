// #define AVR_UNO
#include <AtHome.h>

using AirQualityModule = AtHomeModule<uint8_t, 11>;

#ifdef __AVR__
ArduinoEEPROM storage;
#endif
Stream *streams[] = {&Serial, nullptr};
GroveAirQualitySensor airSensor(A0);
AirQualityModule *module = AirQualityModule::getInstance();
//NeoPixel led(3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  module->setSensor(&airSensor);
  module->setStreams(streams);
  //module->setDisplay(&led);
  #ifdef __AVR__
  module->setStorage(&storage);
  #endif
  module->setCommunicationExecutionInterval(10);
  module->setSensorExecutionInterval(100);
  module->setUploadDataExecutionInterval(1500);
  module->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}
