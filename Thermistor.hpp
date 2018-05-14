#ifndef THERMISTOR_HPP
# define THERMISTOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) && !defined(DISABLE_THERMISTOR_SENSOR)
#  include "ATemperatureSensor.hpp"
#  include "Maths.hpp"
#  ifdef ARDUINO
#   include <Arduino.h>
#  endif /* ARDUINO */

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
            Thermistor(int pin):_pin(pin),_sample(NAN),_increment(5 / PREC) {}
            Thermistor(const Thermistor &) = delete;
            Thermistor &operator=(const Thermistor &) = delete;
            ~Thermistor() {}
            virtual int32_t getSensorSample() {
#  ifdef ARDUINO
                int32_t sample;
                uint16_t adc = analogRead(_pin);
#  else
#   warning Not yet implemented for your platform
#  endif /* ARDUINO */
                float voltage = adc * _increment;
                float resistance = voltage * RREF / (VREF - voltage);
                sample = (1 / (_a + (_b * utility::math::log<float>(resistance)) + (_c * utility::math::pow<float>(utility::math::log<float>(resistance), 3)))) - 273.15;

                return sample;
            }

        private:
            int                     _pin;
            float                   _sample;
            float                   _increment;
        private:
            static constexpr float  _l1 = utility::math::static_log<float>(R1);
            static constexpr float  _l2 = utility::math::static_log<float>(R2);
            static constexpr float  _l3 = utility::math::static_log<float>(R3);
            static constexpr float  _y1 = 1. / 5.;
            static constexpr float  _y2 = 1. / 25.;
            static constexpr float  _y3 = 1. / 50.;
            static constexpr float  _lambda2 = (_y2 - _y1) / (_l2 - _l1);
            static constexpr float  _lambda3 = (_y3 - _y1) / (_l3 - _l1);
            static constexpr float  _c = ((_lambda3 - _lambda2) / (_l3 - _l2)) * (1 / (_l1 + _l2 + _l3));
            static constexpr float  _b = _lambda2 - (_c * (utility::math::static_pow<float>(_l1, 2) + (_l1 * _l2) + utility::math::static_pow<float>(_l2, 2)));
            static constexpr float  _a = _y1 - ((_b + (utility::math::static_pow<float>(_l1, 2) * _c)) * _l1);
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) && !defined(DISABLE_THERMISTOR_SENSOR) */
#endif /* THERMISTOR_HPP */
