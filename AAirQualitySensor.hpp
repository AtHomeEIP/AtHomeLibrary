#ifndef AAIRQUALITYSENSOR_HPP
# define AAIRQUALITYSENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_AIR_QUALITY_SENSOR)
#  include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class AAirQualitySensor : public ISensor {
        public:
            AAirQualitySensor();
            AAirQualitySensor(const AAirQualitySensor &) = delete;
            AAirQualitySensor &operator=(const AAirQualitySensor &) = delete;
            ~AAirQualitySensor();

            virtual uint8_t*        getSample() = 0;
            virtual ISensorScale    getLastSample() const = 0;
            virtual ISensorScale    getEstimate();
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_AIR_QUALITY_SENSOR) */

#endif /* AAIRQUALITYSENSOR_HPP */