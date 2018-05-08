#ifndef GROVEAIRQUALITYSENSOR_HPP
# define GROVEAIRQUALITYSENSOR_HPP

# if defined(__AVR__) && defined(ARDUINO)

#  include <AirQuality.h>
#  include <stdint.h>
#  include "AAirQualitySensor.hpp"

namespace athome {
    namespace sensor {
        class GroveAirQualitySensor : public AAirQualitySensor {
        public:
            GroveAirQualitySensor(uint8_t);
            GroveAirQualitySensor(const GroveAirQualitySensor &) = delete;
            GroveAirQualitySensor &operator=(GroveAirQualitySensor &) = delete;
            ~GroveAirQualitySensor();

            uint8_t                 *getSample();
            ISensor::ISensorScale   getLastSample() const { return getEstimate(); }
            ISensor::ISensorScale   getEstimate() const;

        private:
            uint8_t                 _lastMeasure;
            AirQuality              _sensor;
        };
    }
}

# endif /* List of compatible Arduino boards */

#endif /* GROVEAIRQUALITYSENSOR_HPP */
