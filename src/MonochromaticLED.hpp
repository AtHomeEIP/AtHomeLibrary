#ifndef MONOCHROMATICLED_HPP
#define MONOCHROMATICLED_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_MONOCHROMATIC_LED)
#include "IDisplay.hpp"

namespace athome {
namespace display {
class MonochromaticLED : public IDisplay {
 public:
  MonochromaticLED(int, bool = false);
  MonochromaticLED(const MonochromaticLED &) = delete;
  MonochromaticLED &operator=(const MonochromaticLED &) = delete;
  ~MonochromaticLED();

  virtual void clear();
  virtual void update();
  virtual void setDisplayedEstimate(sensor::ISensor::ISensorScale);

 private:
  int _pin;
  bool _reversed;
  uint8_t _state;
};
}  // namespace display
}  // namespace athome

#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_MONOCHROMATIC_LED) */
#endif /* MONOCHROMATICLED_HPP */
