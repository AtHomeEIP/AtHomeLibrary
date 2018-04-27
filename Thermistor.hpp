#ifndef THERMISTOR_HPP
# define THERMISTOR_HPP

# include <math.h>
# include "ATemperatureSensor.hpp"

namespace athome {
    namespace sensor {
        template <uint32_t R1, uint32_t R2, uint32_t R3>
        class Thermistor : public ATemperatureSensor {
        public:
            Thermistor(int pin):_pin(pin),_sample(NAN) {}
            Thermistor(const Thermistor &) = delete;
            Thermistor &operator=(const Thermistor &) = delete;
            ~Thermistor() {}
            virtual uint8_t *getSample() {return nullptr;}
            virtual float   getLastSample() {return 0;}

        private:
            int     _pin;
            float   _sample;
            float   _a;
            float   _b;
            float   _c;
        };
    }
}

#endif /* THERMISTOR_HPP */
