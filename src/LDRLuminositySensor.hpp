#ifndef LDRLUMINOSITYMODULE_HPP
# define LDRLUMINOSITYMODULE_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && !defined(DISABLE_LDR_LUMINOSITY_SENSOR)
#  include "ALuminositySensor.hpp"
#  include "Maths.hpp"

namespace athome {
    namespace sensor {
        template <uint32_t REF, uint32_t RES, uint32_t RDIV>
        class LDRLuminositySensor : public ALuminositySensor {
        public:
            LDRLuminositySensor(uint8_t pin):_pin(pin) {}
            LDRLuminositySensor(const LDRLuminositySensor &) = delete;
            LDRLuminositySensor &operator=(const LDRLuminositySensor &) = delete;
            ~LDRLuminositySensor() {}

            uint16_t getSensorSample() {
                constexpr uint32_t step = REF / utility::math::static_exp2<uint32_t>(RES);
                constexpr uint16_t vmax = (REF / 1000000) * 5;
                uint16_t value = 0;
# ifdef ARDUINO
                value = ((vmax / analogRead(_pin)) - 500) / (RDIV / 1000);
# endif
                return value;
            }

        private:
            uint8_t     _pin;
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && !defined(DISABLE_LDR_LUMINOSITY_SENSOR) */
#endif /* LDRLUMINOSITYMODULE_HPP */