#include <AtHome.h>

namespace athome {
  namespace sensor {
    class SoundSensor : public ISensor {
    public:
      uint8_t *getSample(){
        sampleValue = digitalRead(8);
        return reinterpret_cast<uint8_t *>(sampleValue);
      }
      ISensorScale getEstimate(){
       return (sampleValue) ? ISensorScale::TEN : ISensorScale::ONE;
     }
   private:
      bool sampleValue;
    };
  }
}

using SoundModule = AtHomeModule<bool, 15>;
using SoundSensor = athome::sensor::SoundSensor;

ArduinoEEPROM storage;
Stream *streams[] = {&Serial, nullptr};
NeoPixel led(6);
SoundSensor soundSensor;
SoundModule *module = SoundModule::getInstance();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    module->setStreams(streams);
    module->setSensor(&soundSensor);
    module->setDisplay(&led);
    module->setCommunicationExecutionInterval(10);
    module->setSensorExecutionInterval(100);
    module->setUploadDataExecutionInterval(1500);
    module->setStorage(&storage);
    module->setup();
}

void loop() {
    // put your main code here, to run repeatedly:
    module->run();
}
