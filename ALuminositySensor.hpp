#ifndef ALUMINOSITYSENSOR_HPP
# define ALUMINOSITYSENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR)
#  include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class ALuminositySensor : public ISensor {
        public:
            ALuminositySensor();
            ALuminositySensor(const ALuminositySensor &) = delete;
            ALuminositySensor &operator=(const ALuminositySensor &) = delete;
            ~ALuminositySensor();

            /**
             * Return a pointer on a uint16_t storing the number of lux as an integer.
             */
            virtual uint8_t         *getSample() = 0;
            /**
             * Return the number of lux as an integer.
             */
            virtual uint16_t        getLastSample() const = 0;
            virtual ISensorScale    getEstimate();
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) */
#endif /* ALUMINOSITYSENSOR_HPP */
