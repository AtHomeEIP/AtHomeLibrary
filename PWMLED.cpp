#include "PWMLED.hpp"

#ifdef ARDUINO
# include <Arduino.h>
#endif

namespace athome {
    namespace display {
        PWMLED::PWMLED(int pin, bool reversed):_pin(pin), _reversed(reversed), _value(0) {
#ifdef ARDUINO
            pinMode(_pin, OUTPUT);
#endif
        }

        PWMLED::~PWMLED() { clear(); }

        void PWMLED::clear() {
#ifdef ARDUINO
            analogWrite(_pin, 0);
#else
# warning Not implemented for your platform
#endif
        }

        void PWMLED::update() {
#ifdef ARDUINO
            analogWrite(_pin, _value);
#else
# warning Not implemented for your platform
#endif
        }

        void PWMLED::setDisplayedEstimate(sensor::ISensor::ISensorScale value) {
            _value = (_reversed) ? value * 25 : 250 - (25 * value);
        }
    }
}
