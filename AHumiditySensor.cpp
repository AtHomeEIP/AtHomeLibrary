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
            int humidity = getLastSample();
            if (humidity < 30 || humidity > 65) {
                return ISensor::ISensorScale::ONE;
            }
            else if (humidity >= 50 && humidity <= 55) {
                return ISensor::ISensorScale::TEN;
            }
            else {
                return ISensor::ISensorScale::FIVE;
            }
        }
    }
}
