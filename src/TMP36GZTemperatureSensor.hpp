#ifndef TMP36GZTEMPERATURESENSOR_HPP
# define TMP36GZTEMPERATURESENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) && !defined(DISABLE_TMP36GZ_TEMPERATURE_SENSOR)
#  ifdef ARDUINO
#   include <Arduino.h>
#  endif /* ARDUNINO */
#  include "ATemperatureSensor.hpp"
#  include "Maths.hpp"

namespace athome {
    namespace sensor {
        template <uint32_t REF, uint8_t RES>
        class TMP36GZTemperatureSensor : public ATemperatureSensor {
        public:
            TMP36GZTemperatureSensor(uint8_t pin):_pin(pin) {}
            TMP36GZTemperatureSensor(const TMP36GZTemperatureSensor &) = delete;
            TMP36GZTemperatureSensor &operator=(const TMP36GZTemperatureSensor &) = delete;
            ~TMP36GZTemperatureSensor() {}

            int32_t getSensorSample() {
#  ifdef ARDUINO
                constexpr uint32_t step = REF / utility::math::static_exp2<uint32_t>(RES);
                int32_t _temp = ((analogRead(_pin) * step) - 500000) * 100;
#  endif /* ARDUINO */
                return _temp;
            }

        private:
            uint8_t     _pin;
        };
    }
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) && !defined(DISABLE_TMP36GZ_TEMPERATURE_SENSOR) */
#endif /* TMP36GZTEMPERATURESENSOR_HPP */