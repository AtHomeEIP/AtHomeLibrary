#ifndef GROVEAIRQUALITYSENSOR_HPP
# define GROVEAIRQUALITYSENSOR_HPP

# ifdef __AVR__

#  include <AirQuality.h>
#  include <stdint.h>
#  include "ISensor.hpp"

namespace woodBox {
    namespace sensor {
        class GroveAirQualitySensor : public ISensor {
        public:
            GroveAirQualitySensor(uint8_t);
            GroveAirQualitySensor(const GroveAirQualitySensor &) = delete;
            GroveAirQualitySensor &operator=(GroveAirQualitySensor &) = delete;
            ~GroveAirQualitySensor();

            uint8_t                 *getSample();
            ISensor::ISensorScale   getEstimate();

        private:
            uint8_t                 _lastMeasure;
            AirQuality              _sensor;
        };
    }
}

# endif /* __AVR__ */

#endif /* GROVEAIRQUALITYSENSOR_HPP */
