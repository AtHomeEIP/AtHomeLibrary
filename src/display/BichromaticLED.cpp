#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_BICHROMATIC_LED)
#include "BichromaticLED.hpp"
#ifdef ARDUINO
#include <Arduino.h>
#endif /* ARDUINO */

namespace athome {
namespace display {
BichromaticLED::BichromaticLED(int good_color, int bad_color, bool reversed)
    : _good_color(good_color), _bad_color(bad_color), _reversed(reversed) {
#ifdef ARDUINO
  pinMode(good_color, OUTPUT);
  pinMode(bad_color, OUTPUT);
#endif /* ARDUINO */
  _state = 0;
}

BichromaticLED::~BichromaticLED() {}

void BichromaticLED::clear() {
#ifdef ARDUINO
  digitalWrite(_good_color, (_reversed) ? HIGH : LOW);
  digitalWrite(_bad_color, (_reversed) ? HIGH : LOW);
#else
#warning Your platform is not yet supported
#endif
}

void BichromaticLED::update() {
  if (_reversed) {
    digitalWrite(_good_color, (_state) ? LOW : HIGH); // Turn on good color if estimate is OK else turn it off
    digitalWrite(_bad_color, (_state) ? HIGH : LOW); // Turn off bad color if estimate is OK else turn it off
  } else {
    digitalWrite(_good_color, (_state) ? HIGH : LOW); // Turn on good color if estimate is OK else turn it off
    digitalWrite(_bad_color, (_state) ? LOW : HIGH); // Turn off bad color if estimate is OK else turn it off
  }
}

void BichromaticLED::setDisplayedEstimate(
    sensor::ISensor::ISensorScale value) {
  _state = (value > sensor::ISensor::ISensorScale::FIVE) ? 1 : 0;
}
}  // namespace display
}  // namespace athome
#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_BICHROMATIC_LED) */
