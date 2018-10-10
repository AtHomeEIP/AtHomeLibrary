#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_PWM_LED)
#include "PWMLED.hpp"
#ifdef ARDUINO
#include <Arduino.h>
#endif

namespace athome {
namespace display {
PWMLED::PWMLED(int pin, bool reversed)
    : _pin(pin), _reversed(reversed), _value(0) {
#ifdef ARDUINO
  pinMode(_pin, OUTPUT);
#endif /* ARDUINO */
}

PWMLED::~PWMLED() { clear(); }

void PWMLED::clear() {
#ifdef ARDUINO
  analogWrite(_pin, 0);
#else
#warning Not implemented for your platform
#endif /* ARDUINO */
}

void PWMLED::update() {
#ifdef ARDUINO
  analogWrite(_pin, _value);
#else
#warning Not implemented for your platform
#endif /* ARDUINO */
}

void PWMLED::setDisplayedEstimate(sensor::ISensor::ISensorScale value) {
  _value = (_reversed) ? value * 25 : 250 - (25 * value);
}
}  // namespace display
}  // namespace athome
#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_PWM_LED) */