#ifndef CHAINABLELED_HPP
#define CHAINABLELED_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_GROVE_CHAINABLE_LED)
#ifdef ARDUINO
#include <ChainableLED.h>
#include "ARGBLed.hpp"

namespace athome {
namespace display {
class GroveChainableLED : public ARGBLed {
 public:
  struct Pins {
    uint8_t clock;
    uint8_t data;
  };
  GroveChainableLED(const Pins * = nullptr);
  GroveChainableLED(const ARGBLed::Color &, const Pins * = nullptr);
  GroveChainableLED(const ARGBLed &, const Pins * = nullptr);
  GroveChainableLED &operator=(const ARGBLed &);
  GroveChainableLED &operator=(const ARGBLed::Color &);
  ~GroveChainableLED();
  virtual void update();
  /* virtual const ARGBLed::Color &getColor() const;
  virtual void setColor(const ARGBLed::Color &); */
 private:
  void setup(const Pins *);

 protected:
  // ARGBLed::Color _color;
  ChainableLED *_led;
};
}  // namespace display
}  // namespace athome

#endif /* GROVE_RGB_CHAINABLE_LED_ENABLED */
#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_GROVE_CHAINABLE_LED) \
        */
#endif /* CHAINABLELED_HPP */
