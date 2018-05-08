#ifndef TMP36GZTEMPERATURESENSOR_HPP
# define TMP36GZTEMPERATURESENSOR_HPP

# ifdef ARDUINO
#  include <Arduino.h>
# endif /* ARDUNINO */
# include "ATemperatureSensor.hpp"
# include "Maths.hpp"

namespace athome {
    namespace sensor {
        template <uint32_t REF, uint8_t RES>
        class TMP36GZTemperatureSensor : public ATemperatureSensor {
        public:
            TMP36GZTemperatureSensor(uint8_t pin):_pin(pin) {}
            TMP36GZTemperatureSensor(const TMP36GZTemperatureSensor &) = delete;
            TMP36GZTemperatureSensor &operator=(const TMP36GZTemperatureSensor &) = delete;
            ~TMP36GZTemperatureSensor() {}

            uint8_t *getSample() {
# ifdef ARDUINO
                constexpr uint32_t step = REF / utility::math::static_exp2<uint32_t>(RES);
                _temp = static_cast<float>(((analogRead(_pin) * step) - 500000)) / 10000.;
# endif /* ARDUINO */
                return reinterpret_cast<uint8_t *>(&_temp);
            }
            float   getLastSample() const {
                return _temp;
            }

        private:
            uint8_t     _pin;
            float       _temp;
        };
    }
}

#endif /* TMP36GZTEMPERATURESENSOR_HPP */