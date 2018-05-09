#ifndef LDRLUMINOSITYMODULE_HPP
# define LDRLUMINOSITYMODULE_HPP

# include "ALuminositySensor.hpp"
# include "Maths.hpp"

namespace athome {
    namespace sensor {
        template <uint32_t REF, uint32_t RES, uint32_t RDIV>
        class LDRLuminositySensor : public ALuminositySensor {
        public:
            LDRLuminositySensor(uint8_t pin):_pin(pin) {}
            LDRLuminositySensor(const LDRLuminositySensor &) = delete;
            LDRLuminositySensor &operator=(const LDRLuminositySensor &) = delete;
            ~LDRLuminositySensor() {}

            uint8_t *getSample() {
                constexpr uint32_t step = REF / utility::math::static_exp2<uint32_t>(RES);
                constexpr uint16_t vmax = (REF / 1000000) * 5;
# ifdef ARDUINO
                _value = ((vmax / analogRead(_pin)) - 500) / (RDIV / 1000);
# endif
                return reinterpret_cast<uint8_t *>(&_value);
            }
            uint16_t    getLastSample() const {
                return _value;
            }

        private:
            uint8_t     _pin;
            uint16_t    _value;
        };
    }
}

#endif /* LDRLUMINOSITYMODULE_HPP */