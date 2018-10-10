#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_GROVE_CHAINABLE_LED)
#ifdef ARDUINO
#include "GroveChainableLED.hpp"

namespace athome {
namespace display {
GroveChainableLED::GroveChainableLED(const Pins *pins) : _led(nullptr) {
  setup(pins);
}

GroveChainableLED::GroveChainableLED(const ARGBLed::Color &color,
                                     const Pins *pins)
    : _led(nullptr) {
  setup(pins);
}

GroveChainableLED::GroveChainableLED(const ARGBLed &other, const Pins *pins)
    : _led(nullptr) {
  setup(pins);
}

GroveChainableLED &GroveChainableLED::operator=(const ARGBLed::Color &color) {
  setColor(color);
  return *this;
}

GroveChainableLED &GroveChainableLED::operator=(const ARGBLed &other) {
  setColor(other.getColor());
  return *this;
}

GroveChainableLED::~GroveChainableLED() {
  if (_led != nullptr) delete _led;
}

void GroveChainableLED::update() {
  const ARGBLed::Color &color = getColor();
  _led->setColorRGB(0, color.red, color.green, color.blue);
}

/* const ARGBLed::Color &GroveChainableLED::getColor() const {
        return _color;
}

void GroveChainableLED::setColor(const ARGBLed::Color &color) {
        _color = color;
} */

void GroveChainableLED::setup(const Pins *pins) {
  if (_led != nullptr) delete _led;
  _led = (pins != nullptr) ? new ChainableLED(pins->clock, pins->data, 1)
                           : new ChainableLED(7, 8, 1);
  update();
}
}  // namespace display
}  // namespace athome

#endif /* GROVE_RGB_CHAINABLE_LED_ENABLED */
#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_GROVE_CHAINABLE_LED) \
        */