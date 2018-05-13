#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR)
# include "ALuminositySensor.hpp"

namespace athome {
    namespace sensor {
        ALuminositySensor::ALuminositySensor() {}

        ALuminositySensor::~ALuminositySensor() {}

        /**
         * Return an estimation of the luminosity quality in the room
         *
         * Values are from french norm NF EN 12464-1 https://fr.wikipedia.org/wiki/Lux_%28unit%C3%A9%29
         * @return estimation of the luminosity on a scale from 1 (worst) to 10 (best)
         */
        ISensor::ISensorScale ALuminositySensor::getEstimate() {
            uint16_t lux = getLastSample();
            if (lux < 500) {
                return ISensor::ISensorScale::ONE;
            }
            else {
                return ISensor::ISensorScale::TEN;
            }
        }
    }
}

#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) */