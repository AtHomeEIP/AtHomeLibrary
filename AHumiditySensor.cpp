#include "AHumiditySensor.hpp"

namespace athome {
    namespace sensor {
        AHumiditySensor::AHumiditySensor() {}

        AHumiditySensor::~AHumiditySensor() {}

        ISensor::ISensorScale AHumiditySensor::getEstimate() {
            // TODO: What are the values???
            return ISensor::ISensorScale::ZERO;
        }
    }
}
