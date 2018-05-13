#ifndef ATHOME_ANOISESENSOR_HPP
# define ATHOME_ANOISESENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_NOISE_SENSOR)
#  include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class ANoiseSensor : public ISensor {
        public:
            ANoiseSensor();
            ANoiseSensor(const ANoiseSensor &) = delete;
            ANoiseSensor &operator=(const ANoiseSensor &) = delete;
            ~ANoiseSensor();

            virtual uint8_t *getSample() = 0;
            virtual int32_t getLastSample() const = 0;
            ISensorScale    getEstimate();
        };
    }
}

# endif /*  !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR) */
#endif //ATHOME_ANOISESENSOR_HPP
