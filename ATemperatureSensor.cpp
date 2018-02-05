#include "ATemperatureSensor.hpp"

namespace athome {
    namespace sensor {
        ATemperatureSensor::ATemperatureSensor() {}

        ATemperatureSensor::~ATemperatureSensor() {}

        ISensor::ISensorScale ATemperatureSensor::getEstimate() {
            // TODO: What are the values???
            return ISensor::ISensorScale::ZERO;
        }
    }
}
