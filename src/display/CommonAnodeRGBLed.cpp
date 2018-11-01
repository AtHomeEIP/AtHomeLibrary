#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_COMMON_ANODE_RGB_LED)
#include "CommonAnodeRGBLed.hpp"
#ifdef ARDUINO
#include <Arduino.h>
#else
extern void analogWrite(uint8_t, int);
#endif

namespace athome {
namespace display {
CommonAnodeRGBLed::CommonAnodeRGBLed(uint8_t pin_red, uint8_t pin_green,
                                     uint8_t pin_blue)
    : ARGBLed(),
      _pin_red(pin_red),
      _pin_green(pin_green),
      _pin_blue(pin_blue) {}

CommonAnodeRGBLed &CommonAnodeRGBLed::operator=(
    const CommonAnodeRGBLed &other) {
  if (&other != this) {
    setColor(other.getColor());
  }
  return *this;
}

CommonAnodeRGBLed::~CommonAnodeRGBLed() {}

void CommonAnodeRGBLed::update() {
  const Color &color = getColor();
  analogWrite(255 - _pin_red, color.red);
  analogWrite(255 - _pin_green, color.green);
  analogWrite(255 - _pin_blue, color.blue);
}
}  // namespace display
}  // namespace athome
#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_COMMON_ANODE_RGB_LED) \
        */
