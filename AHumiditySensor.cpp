#include "AHumiditySensor.hpp"

namespace athome {
    namespace sensor {
        AHumiditySensor::AHumiditySensor() {}

        AHumiditySensor::~AHumiditySensor() {}

        /**
         * Return an estimation of the humidity inside the room
         *
         * Values are from https://www.jechange.fr/energie/duale/guides/thermostat-temperature-4101
         * @return estimation of humidity quality on a scale from 1 (worst) to 10 (best)
         */
        ISensor::ISensorScale AHumiditySensor::getEstimate() {
            // Compute using 10^-6 values
            int32_t humidity = getLastSample();
            if (humidity < 30000000 || humidity > 65000000) {
                return ISensor::ISensorScale::ONE;
            }
            else if (humidity >= 50000000 && humidity <= 55000000) {
                return ISensor::ISensorScale::TEN;
            }
            else {
                return ISensor::ISensorScale::FIVE;
            }
        }
    }
}
