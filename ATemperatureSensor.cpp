#include "ATemperatureSensor.hpp"

namespace athome {
    namespace sensor {
        ATemperatureSensor::ATemperatureSensor() {}

        ATemperatureSensor::ATemperatureSensor(const ATemperatureSensor &other) {
            (void)other;
        }

        ATemperatureSensor &ATemperatureSensor::operator=(const ATemperatureSensor &other) {
            (void)other;
            return *this;
        }

        ATemperatureSensor::~ATemperatureSensor() {}

        ISensor::ISensorScale ATemperatureSensor::getEstimate() {
            // TODO: What are the values???
            return ISensor::ISensorScale::ZERO;
        }
    }
}
