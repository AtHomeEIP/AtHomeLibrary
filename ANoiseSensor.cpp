#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR)
# include "ANoiseSensor.hpp"

namespace athome {
    namespace sensor {
        ANoiseSensor::ANoiseSensor() {}

        ANoiseSensor::~ANoiseSensor() {}

        ISensor::ISensorScale ANoiseSensor::getEstimate() {
            return ISensor::ISensorScale::ZERO;
        }
    }
}

#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR) */