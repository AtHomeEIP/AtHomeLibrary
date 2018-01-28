#ifndef GROVEAIRQUALITYSENSOR_HPP
# define GROVEAIRQUALITYSENSOR_HPP

#if defined(AVR_PRO) || defined(AVR_UNO_WIFI_DEV_ED) || defined(AVR_NG) || \
        defined(AVR_LILYPAD) || defined(AVR_BT) || defined(AVR_FIO) || \
        defined(AVR_ETHERNET) || defined(AVR_MINI) || defined(AVR_NANO) || \
        defined(AVR_DUEMILANOVE) || defined(AVR_UNO)

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

# endif /* List of compatible Arduino boards */

#endif /* GROVEAIRQUALITYSENSOR_HPP */
