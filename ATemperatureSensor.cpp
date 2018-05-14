#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR)
# include "ATemperatureSensor.hpp"

namespace athome {
    namespace sensor {
        ATemperatureSensor::ATemperatureSensor():_value{0},_temp(0) {
            _value.sampleRawPointer = reinterpret_cast<void *>(&_temp);
            _value.unit.unit = utility::units::si::UNIT::DEGREE_CELSIUS;
            _value.unit.prefix = utility::units::si::PREFIX::MICRO;
        }

        ATemperatureSensor::~ATemperatureSensor() {}

        /**
         * Return an estimation of the quality of the temperature
         *
         * Values are from: https://www.jechange.fr/energie/duale/guides/thermostat-temperature-4101
         * @return estimation of temperature on a scale from 1 (worst) to 10 (best)
         */
        const ISensor::ISensorValue &ATemperatureSensor::getSample() {
            _temp = getSensorSample();
            if (_temp < 16000000) {
                _value.estimate = ISensor::ISensorScale::ONE;
            }
            else if (_temp >= 16000000 && _temp <= 18000000) {
                _value.estimate = ISensor::ISensorScale::FOUR;
            }
            else {
                _value.estimate = ISensor::ISensorScale::TEN;
            }
            return _value;
        }

        void ATemperatureSensor::setThresholds(const ISensorThresholds &thresholds) {
            (void)thresholds;
        }
    }
}
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) */