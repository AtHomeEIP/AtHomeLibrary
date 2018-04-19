#ifndef SOUNDSENSOR_HPP
# define SOUNDSENSOR_HPP

# include <ISensor.hpp>

# ifdef ARDUINO

namespace athome {
  namespace sensor {
    class SoundSensor : public ISensor {
    public:
      SoundSensor(int pin):_pin(pin),_sampleValue(false) {}
      SoundSensor(const SoundSensor &) = delete; // Has no meaning
      SoundSensor &operator=(const SoundSensor &) = delete; // Has no meaning
      ~SoundSensor() {}
      uint8_t *getSample(){ return reinterpret_cast<uint8_t *>((sampleValue = digitalRead(_pin))); }
      ISensorScale getEstimate(){ return (_sampleValue) ? ISensorScale::TEN : ISensorScale::ONE; }
      void setPin(int pin) { _pin = pin; }
      int getPin() const { return _pin; }
    private:
      int _pin;
      bool _sampleValue;
    };
  }
}

# endif /* ARDUINO */
#endif /* SOUNDSENSOR_HPP */