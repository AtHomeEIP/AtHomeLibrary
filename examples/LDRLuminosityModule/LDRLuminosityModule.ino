#include <AtHome.h>

using LuminosityModule = AtHomeModule<int16_t, 1>;

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
FakeRTC rtc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  module->setStreams(streams);
  module->setSensor(&ldrSensor);
  module->setDisplay(&led);
  module->setStorage(storage);
  //module->setSerial(0); // Uncomment to reinitialize the id of the module
  module->setClock(&rtc);
  module->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}
