#include "ALuminositySensor.hpp"

namespace athome {
    namespace sensor {
        ALuminositySensor::ALuminositySensor() {}

        ALuminositySensor::~ALuminositySensor() {}

        ISensor::ISensorScale ALuminositySensor::getEstimate() {
            // TODO: Seriously, what are the values to use??? We didn't put them in Arduino sketches
            return ISensor::ISensorScale::ZERO;
        }
    }
}
