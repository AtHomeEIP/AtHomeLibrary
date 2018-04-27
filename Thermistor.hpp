#ifndef THERMISTOR_HPP
# define THERMISTOR_HPP

# include <math.h>
# include "ATemperatureSensor.hpp"

# ifdef ARDUINO
#  include <Arduino.h>
# endif /* ARDUINO */

namespace athome {
    namespace sensor {
        /**
         * Based on Steinhart-Hart equation:
         *
         * - T1 = 5 degree celsius
         * - T2 = 25 degree celsius
         * - T3 = 50 degree celsius
         *
         * TODO: Need to update it to use the static version of log function once implemented
         */
        template <uint32_t R1, uint32_t R2, uint32_t R3, size_t PREC = 1024, uint8_t VREF = 5, uint32_t RREF = 10000>
        class Thermistor : public ATemperatureSensor {
        public:
            Thermistor(int pin):_pin(pin),_sample(NAN),_increment(5 / PREC) {
                float l1 = log(R1);
                float l2 = log(R2);
                float l3 = log(R3);
                float y1 = 1. / 5.;
                float y2 = 1. / 25.;
                float y3 = 1. / 50.;
                float lambda2 = (y2 - y1) / (l2 - l1);
                float lambda3 = (y3 - y1) / (l3 - l1);

                _c = ((lambda3 - lambda2) / (l3 - l2)) * (1 / (l1 + l2 + l3));
                _b = lambda2 - (_c * (pow(l1, 2) + (l1 * l2) + pow(l2, 2)));
                _a = y1 - ((_b + (pow(l1, 2) * _c)) * l1);
            }
            Thermistor(const Thermistor &) = delete;
            Thermistor &operator=(const Thermistor &) = delete;
            ~Thermistor() {}
            virtual uint8_t *getSample() {
# ifdef ARDUINO
                uint16_t adc = analogRead(_pin);
# else
#  warning Not yet implemented for your platform
# endif /* ARDUINO */
                float voltage = adc * _increment;
                float resistance = voltage * RREF / (VREF - voltage);
                _sample = (1 / (_a + (_b * log(resistance)) + (_c * pow(log(resistance), 3)))) - 273.15;

                return reinterpret_cast<uint8_t *>(&_sample);
            }
            virtual float   getLastSample() const { return _sample; }

        private:
            int                     _pin;
            float                   _sample;
            float                   _increment;
        private:
            float  _c;
            float  _b;
            float  _a;
        };
    }
}

#endif /* THERMISTOR_HPP */
