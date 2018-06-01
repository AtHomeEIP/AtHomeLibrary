#if !defined(DISABLE_TIME) || !defined(DISABLE_DS1307)
# include "DS1307.hpp"

namespace athome {
    namespace time {
        DS1307::DS1307():_wire(Wire) {}

        DS1307::DS1307(TwoWire &wire):_wire(wire) {}

        DS1307::~DS1307() {}

        const ITime::ISO8601DateTime &DS1307::getDateTime() {
            if (!_initialized) {
                return _date;
            }
            // Algorithm from Adafruit:
            _wire.beginTransmission(DS1307_ADDRESS);
            _wire.write(static_cast<byte>(0));
            _wire.endTransmission();
            _wire.requestFrom(DS1307_ADDRESS, 7);
            _date.second = bcd2bin(_wire.read() & 0x7F);
            _date.minute = bcd2bin(_wire.read());
            _date.hour = bcd2bin(_wire.read());
            _wire.read();
            _date.day = bcd2bin(_wire.read());
            _date.month = bcd2bin(_wire.read());
            _date.year = bcd2bin(_wire.read());
            return _date;
        }

        void DS1307::setCurrentDateTime(const ITime::DateTime &time) {
            if (!_initialized) {
                return;
            }
            // Algorithm from Adafruit:
            _wire.beginTransmission(DS1307_ADDRESS);
            _wire.write(static_cast<byte>(0));
            _wire.write(bin2bcd(time.second));
            _wire.write(bin2bcd(time.minute));
            _wire.write(bin2bcd(time.hour));
            _wire.write(bin2bcd(0));
            _wire.write(bin2bcd(time.day));
            _wire.write(bin2bcd(time.month));
            _wire.write(bin2bcd(time.year));
            _wire.endTransmission();
        }

        void DS1307::setup() {
            _wire.begin();
            _initialized = is_running();
        }

        // Algorithm from Adafruit: https://github.com/adafruit/RTClib/blob/master/RTClib.cpp
        bool DS1307::is_running() {
            _wire.beginTransmission(DS1307_ADDRESS);
            _wire.write(static_cast<byte>(0));
            _wire.endTransmission();
            _wire.requestFrom(DS1307_ADDRESS, 1);
            uint8_t res = _wire.read();
            return !(res>>7);
        }

        // Algorithm from Adafruit:
        uint8_t DS1307::bcd2bin(uint8_t val) { return val - 6 * (val >> 4); }

        // Algorithm from Adafruit:
        uint8_t DS1307::bin2bcd(uint8_t val) { return val + 6 * (val / 10); }
    }
}

#endif /* !defined(DISABLE_TIME) || !defined(DISABLE_DS1307) */