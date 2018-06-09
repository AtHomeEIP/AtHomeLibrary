#ifndef DS1307_HPP
# define DS1307_HPP
# include "AtHomeConfig.h"
# if !defined(DISABLE_TIME) && !defined(DISABLE_DS1307)
// Values found from Adafruit RTClib (https://github.com/adafruit/RTClib/blob/master/RTClib.h):
#  define DS1307_ADDRESS    0x68
#  define DS1307_CONTROL    0x07
#  define DS1307_NVRAM      0x08

#  include <Arduino.h>
#  include <Wire.h>
#  include "ITime.hpp"

namespace athome {
    namespace time {
        class DS1307 : public ITime {
        public:
            DS1307();
            DS1307(TwoWire &);
            DS1307(const DS1307 &) = delete;
            DS1307 &operator=(const DS1307 &) = delete;
            ~DS1307();

            virtual const ITime::ISO8601DateTime &getDateTime();
            virtual void setCurrentDateTime(const DateTime &);

        protected:
            void setup();
            bool is_running();
            uint8_t bcd2bin(uint8_t);
            uint8_t bin2bcd(uint8_t);

        private:
            TwoWire &_wire;
            bool _initialized;
            ISO8601DateTime _date;
        };
    }
}

# endif /* !defined(DISABLE_TIME) && !defined(DISABLE_DS1307) */
#endif /* DS1307_HPP */