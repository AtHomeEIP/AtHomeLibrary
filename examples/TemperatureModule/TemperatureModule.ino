#include <AtHome.h>

using TemperatureModule = AtHomeModule<int32_t, 1>;

Stream *streams[] = {&Serial, nullptr};
TMP36GZTemperatureSensor3V3 tempSensor(A6);
TemperatureModule *module = TemperatureModule::getInstance();
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
  module->setSensor(&tempSensor);
  module->setDisplay(&led);
  module->setStorage(storage);
  module->setClock(&rtc);
  //module->setSerial(0); // Uncomment to reinitialize the id of the module
  module->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}