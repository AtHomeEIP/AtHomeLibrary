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
             * Return a reference on ISensorValue storing the number of lux as an integer.
             */
            const ISensorValue  &getSample();
            /**
             * Return the number of lux as an integer.
             */
            virtual uint16_t    getSensorSample() = 0;
            void                setThresholds(const ISensorThresholds &);

        private:
            ISensorValue    _value;
            uint16_t        _lux;
            uint16_t        _min;
            uint16_t        _max;
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) */
#endif /* ALUMINOSITYSENSOR_HPP */
