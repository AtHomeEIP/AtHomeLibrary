#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_AIR_QUALITY_SENSOR)

# include "AAirQualitySensor.hpp"

namespace athome {
    namespace sensor {
        AAirQualitySensor::AAirQualitySensor() {}

        AAirQualitySensor::~AAirQualitySensor() {}

        ISensor::ISensorScale AAirQualitySensor::getEstimate() {
            return getLastSample();
        }
    }
}

#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_AIR_QUALITY_SENSOR) */