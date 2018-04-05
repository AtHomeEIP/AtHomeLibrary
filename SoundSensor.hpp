#include <ISensor.hpp>

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
