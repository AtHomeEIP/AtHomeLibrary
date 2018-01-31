#include "AHumiditySensor.hpp"

namespace athome {
    namespace sensor {
        AHumiditySensor::AHumiditySensor() {}

        AHumiditySensor::AHumiditySensor(const AHumiditySensor &other) {
            (void)other;
        }

        AHumiditySensor &AHumiditySensor::operator=(const AHumiditySensor &other) {
            (void)other;
            return *this;
        }

        AHumiditySensor::~AHumiditySensor() {}

        ISensor::ISensorScale AHumiditySensor::getEstimate() {
            // TODO: What are the values???
            return ISensor::ISensorScale::ZERO;
        }
    }
}
