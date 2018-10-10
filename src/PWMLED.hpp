#ifndef PWMLED_HPP
#define PWMLED_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_DISPLAY) && !defined(DISABLE_PWM_LED)
#include "IDisplay.hpp"

namespace athome {
namespace display {
class PWMLED : public IDisplay {
 public:
  PWMLED(int, bool = false);
  PWMLED(const PWMLED &) = delete;
  PWMLED &operator=(const PWMLED &) = delete;
  ~PWMLED();

  virtual void clear();
  virtual void update();
  virtual void setDisplayedEstimate(sensor::ISensor::ISensorScale);

 private:
  int _pin;
  bool _reversed;
  uint8_t _value;
};
}  // namespace display
}  // namespace athome

#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_PWM_LED) */
#endif /* PWMLED_HPP */
