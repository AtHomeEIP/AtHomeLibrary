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
                SoundSensor(int pin):
                        _pin(pin),
                        _sampleValue(false),
                        _value({
                                ISensorScale::ZERO,
                                {
                                    utility::units::UNIT::UNKNOWN,
                                    utility::units::PREFIX::BASE_UNIT
                                },
                                reinterpret_cast<void *>(&_sampleValue),
                                PSTR("Binary noise detector")
                        }) {}
                SoundSensor(const SoundSensor &) = delete; // Has no meaning
                SoundSensor &operator=(const SoundSensor &) = delete; // Has no meaning
                ~SoundSensor() {}

                const ISensorValue &getSample() {
                    _sampleValue = digitalRead(_pin);
                    _value.estimate = (_sampleValue) ? ISensorScale::TEN : ISensorScale::ONE;
                    return _value;
                }
                void setThresholds(const ISensorThresholds &thresholds) { (void)thresholds; }
                void setPin(int pin) { _pin = pin; }
                int getPin() const { return _pin; }

            private:
                int             _pin;
                bool            _sampleValue;
                ISensorValue    _value;
        };
    }
}

#  endif /* ARDUINO */
# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_NOISE_SENSOR) && !defined(DISABLE_SOUND_SENSOR) */
#endif /* SOUNDSENSOR_HPP */