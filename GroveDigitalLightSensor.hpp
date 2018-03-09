#ifndef GROVEDIGITALLIGHTSENSOR_HPP
# define GROVEDIGITALLIGHTSENSOR_HPP

# ifdef ARDUINO
#  include <stdint.h>
#  include "ALuminositySensor.hpp"

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

# endif /* ARDUINO */

#endif /* GROVEDIGITALLIGHTSENSOR_HPP */