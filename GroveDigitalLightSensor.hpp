#ifndef GROVEDIGITALLIGHTSENSOR_HPP
# define GROVEDIGITALLIGHTSENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR)
#  if defined(ARDUINO) && (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) ||\
      defined(__AVR_ATmega2560__))
#   include <stdint.h>
#   include "ALuminositySensor.hpp"

namespace athome {
    namespace sensor {
        class GroveDigitalLightSensor : public athome::sensor::ALuminositySensor {
        public:
            GroveDigitalLightSensor(const GroveDigitalLightSensor &) = delete;
            GroveDigitalLightSensor &operator=(const GroveDigitalLightSensor &) = delete;

            static GroveDigitalLightSensor  *getInstance();
            uint8_t                         *getSample();
            uint16_t                        getLastSample() const;
        protected:
            GroveDigitalLightSensor();
        private:
            ~GroveDigitalLightSensor();
        private:
            uint16_t                        _sample;
        private:
            static GroveDigitalLightSensor  *_instance;
        };
    }
}

#  endif /* ARDUINO */
# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR) */
#endif /* GROVEDIGITALLIGHTSENSOR_HPP */