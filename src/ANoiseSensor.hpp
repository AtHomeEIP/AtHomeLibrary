#ifndef ATHOME_ANOISESENSOR_HPP
#define ATHOME_ANOISESENSOR_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_NOISE_SENSOR)
#include "ISensor.hpp"

namespace athome {
namespace sensor {
class ANoiseSensor : public ISensor {
 public:
  ANoiseSensor();
  ANoiseSensor(const ANoiseSensor &) = delete;
  ANoiseSensor &operator=(const ANoiseSensor &) = delete;
  ~ANoiseSensor();

  const ISensorValue &getSample();
  virtual int32_t getSensorSample() const = 0;
  void setThresholds(const ISensorThresholds &);

 private:
  ISensorValue _value;
  int32_t _noise;
  int32_t _min;
  int32_t _max;
};
}  // namespace sensor
}  // namespace athome

#endif  /*  !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR) */
#endif  // ATHOME_ANOISESENSOR_HPP
