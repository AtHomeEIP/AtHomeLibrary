#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMON_CATHODE_RGB_LED) && !defined(DISABLE_DISPLAY)
#ifdef ARDUINO
#include <Arduino.h>
#else
extern void analogWrite(uint8_t, int);
#endif

#include "CommonCathodeRGBLed.hpp"

namespace athome {
namespace display {
CommonCathodeRGBLed::CommonCathodeRGBLed(uint8_t pin_red, uint8_t pin_green,
                                         uint8_t pin_blue)
    : ARGBLed(),
      _pin_red(pin_red),
      _pin_green(pin_green),
      _pin_blue(pin_blue) {}

CommonCathodeRGBLed &CommonCathodeRGBLed::operator=(
    const CommonCathodeRGBLed &other) {
  setColor(other.getColor());
  return *this;
}

CommonCathodeRGBLed::~CommonCathodeRGBLed() {}

void CommonCathodeRGBLed::update() {
  const Color &color = getColor();
  analogWrite(_pin_red, color.red);
  analogWrite(_pin_green, color.green);
  analogWrite(_pin_blue, color.blue);
}
}  // namespace display
}  // namespace athome
#endif /* !defined(DISABLE_COMMON_CATHODE_RGB_LED) && \
          !defined(DISABLE_DISPLAY) */