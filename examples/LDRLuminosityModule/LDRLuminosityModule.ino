#include <AtHome.h>

using LuminosityModule = AtHomeModule<int16_t, 11>;

Stream *streams[] = {&Serial, nullptr};
LDRLuminositySensor3V3 ldrSensor(A6);
LuminosityModule *module = LuminosityModule::getInstance();
MonochromaticLED led(2, true);
#ifdef __MSP430__
MSP430FRAM *storage = MSP430FRAM::getInstance();
#elif defined(__AVR__)
ArduinoEEPROM eeprom;
IStorage *storage = &eeprom;
#else
IStorage *storage = nullptr;
#endif /* __MSP430__ */

void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  module->setStreams(streams);
  module->setSensor(&ldrSensor);
  module->setDisplay(&led);
  module->setStorage(storage);
  module->setCommunicationExecutionInterval(1);
  module->setSensorExecutionInterval(100);
  module->setUploadDataExecutionInterval(1001);
  module->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}
