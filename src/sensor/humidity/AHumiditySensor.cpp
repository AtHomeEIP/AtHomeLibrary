#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_HUMIDITY_SENSOR)
# include "AHumiditySensor.hpp"

# define MIN_HUMIDITY 30000000
# define MAX_HUMIDITY 65000000

namespace athome {
    namespace sensor {
        AHumiditySensor::AHumiditySensor():_value({
                ISensor::ISensorScale::ZERO,
                {
                        utility::units::UNIT::RELATIVE_HUMIDITY,
                        utility::units::PREFIX::MICRO
                },
                reinterpret_cast<void *>(&_humidity),
                PSTR("Humidity")
        }),
        _humidity(0),
        _min(50000000),
        _max(55000000) {}

        AHumiditySensor::~AHumiditySensor() {}

        /**
         * Return an estimation of the humidity inside the room
         *
         * Values are from https://www.jechange.fr/energie/duale/guides/thermostat-temperature-4101
         * @return estimation of humidity quality on a scale from 1 (worst) to 10 (best)
         */
        const ISensor::ISensorValue &AHumiditySensor::getSample() {
            // Compute using 10^-6 values
            _humidity = getSensorSample();
            if (_humidity < 30000000 || _humidity > 65000000) {
                _value.estimate = ISensor::ISensorScale::ONE;
            }
            else if (_humidity >= _min && _humidity <= _max) {
                _value.estimate = ISensor::ISensorScale::TEN;
            }
            else {
                _value.estimate = ISensor::ISensorScale::FIVE;
            }
            return _value;
        }

        void AHumiditySensor::setThresholds(const ISensor::ISensorThresholds &thresholds) {
            if (thresholds.unit.unit != utility::units::UNIT::RELATIVE_HUMIDITY) {
                return;
            }
            _min = thresholds.min;
            _max = thresholds.max;
            if (thresholds.unit.prefix == utility::units::PREFIX::BASE_UNIT) {
                _min *= 1000000;
                _max *= 1000000;
            }
        }
    }
}
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_HUMIDITY_SENSOR) */