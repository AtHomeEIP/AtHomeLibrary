#include <woodBox.h>

#ifdef __AVR__

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

ArduinoEEPROM storage;

#endif

using SimpleModule = WoodBoxModule<uint16_t, 15>;

SimpleModule *module = SimpleModule::getInstance();
Stream *streams[] = {&Serial, nullptr};
AnalogSensor sensor(A0);

void setup() {
  // put your setup code here, to run once
  Serial.begin(115200);
  module->setStreams(streams);
  module->setSensor(&sensor);
  module->setSensorExecutionInterval(1000);
  module->setCommunicationExecutionInterval(1);
#ifdef __AVR__
  module->setStorage(&storage);
  Serial.println(freeRam());
#endif
}

void loop() {
  // put your main code here, to run repeatedly:
  module->run();
}
