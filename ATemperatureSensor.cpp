#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR)
# include "ATemperatureSensor.hpp"

namespace athome {
    namespace sensor {
        ATemperatureSensor::ATemperatureSensor() {}

        ATemperatureSensor::~ATemperatureSensor() {}

        /**
         * Return an estimation of the quality of the temperature
         *
         * Values are from: https://www.jechange.fr/energie/duale/guides/thermostat-temperature-4101
         * @return estimation of temperature on a scale from 1 (worst) to 10 (best)
         */
        ISensor::ISensorScale ATemperatureSensor::getEstimate() {
            int32_t temp = getLastSample();
            if (temp < 16000000) {
                return ISensor::ISensorScale::ONE;
            }
            else if (temp >= 16000000 && temp <= 18000000) {
                return ISensor::ISensorScale::FOUR;
            }
            else {
                return ISensor::ISensorScale::TEN;
            }
        }
    }
}
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) */