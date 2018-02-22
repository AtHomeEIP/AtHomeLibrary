#ifndef AAIRQUALITYSENSOR_HPP
# define AAIRQUALITYSENSOR_HPP

# include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class AAirQualitySensor : public ISensor {
        public:
            AAirQualitySensor();
            AAirQualitySensor(const AAirQualitySensor &) = delete;
            AAirQualitySensor &operator=(const AAirQualitySensor &) = delete;
            ~AAirQualitySensor();

            virtual uint8_t*        getSample() = 0;
            virtual ISensorScale    getLastSample() = 0;
            virtual ISensorScale    getEstimate();
        };
    }
}

#endif /* AAIRQUALITYSENSOR_HPP */