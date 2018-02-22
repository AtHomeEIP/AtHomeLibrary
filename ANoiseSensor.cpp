#include "ANoiseSensor.hpp"

namespace athome {
    namespace sensor {
        ANoiseSensor::ANoiseSensor() {}

        ANoiseSensor::~ANoiseSensor() {}

        ISensor::ISensorScale ANoiseSensor::getEstimate() {
            return ISensor::ISensorScale::ZERO;
        }
    }
}