#include "MonochromaticLED.hpp"

#ifdef ARDUINO
# include <Arduino.h>
#endif

namespace athome {
    namespace display {
        MonochromaticLED::MonochromaticLED(int pin, bool reversed):_pin(pin), _reversed(reversed) {
            _state = (_reversed) ? HIGH : LOW;
        }

        MonochromaticLED::~MonochromaticLED() {}

        void MonochromaticLED::clear() {
#ifdef ARDUINO
            digitalWrite(_pin, (_reversed) ? HIGH : LOW);
#else
# warning Your platform is not yet supported
#endif
        }

        void MonochromaticLED::update() {
            digitalWrite(_pin, (_reversed) ? !_state : _state);
        }

        void MonochromaticLED::setDisplayedEstimate(sensor::ISensor::ISensorScale value) {
            _state = (value > sensor::ISensor::ISensorScale::FIVE) ? HIGH : LOW;
        }
    }
}
