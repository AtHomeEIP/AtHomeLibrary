#ifndef SOUNDSENSOR_HPP
# define SOUNDSENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_NOISE_SENSOR) && !defined(DISABLE_SOUND_SENSOR)
#  include "ISensor.hpp"
#  ifdef ARDUINO

namespace athome {
  namespace sensor {
    class SoundSensor : public ISensor {
    public:
      SoundSensor(int pin):_pin(pin),_sampleValue(false) {}
      SoundSensor(const SoundSensor &) = delete; // Has no meaning
      SoundSensor &operator=(const SoundSensor &) = delete; // Has no meaning
      ~SoundSensor() {}
      uint8_t *getSample(){ return reinterpret_cast<uint8_t *>(&(_sampleValue = digitalRead(_pin))); }
      ISensorScale getEstimate(){ return (_sampleValue) ? ISensorScale::TEN : ISensorScale::ONE; }
      void setPin(int pin) { _pin = pin; }
      int getPin() const { return _pin; }
      int _pin;
    private:
      bool _sampleValue;
    };
  }
}

#  endif /* ARDUINO */
# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_NOISE_SENSOR) && !defined(DISABLE_SOUND_SENSOR) */
#endif /* SOUNDSENSOR_HPP */