#ifndef BICHROMATICLED_HPP
#define BICHROMATICLED_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_BICHROMATIC_LED)
#include "IDisplay.hpp"

namespace athome {
namespace display {
class BichromaticLED : public IDisplay {
 public:
  BichromaticLED(int, int, bool = false);
  BichromaticLED(const BichromaticLED &) = delete;
  BichromaticLED &operator=(const BichromaticLED &) = delete;
  ~BichromaticLED();

  virtual void clear();
  virtual void update();
  virtual void setDisplayedEstimate(sensor::ISensor::ISensorScale);

 private:
  int _good_color;
  int _bad_color;
  bool _reversed;
  uint8_t _state;
};
}  // namespace display
}  // namespace athome

#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_BICHROMATIC_LED) */
#endif /* BICHROMATICLED_HPP */
