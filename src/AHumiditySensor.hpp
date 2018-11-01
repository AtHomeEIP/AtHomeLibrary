#ifndef AHUMIDITYSENSOR_HPP
#define AHUMIDITYSENSOR_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_HUMIDITY_SENSOR)
#include "ISensor.hpp"

namespace athome {
namespace sensor {
class AHumiditySensor : public ISensor {
 public:
  AHumiditySensor();
  AHumiditySensor(const AHumiditySensor &) = delete;
  AHumiditySensor &operator=(const AHumiditySensor &) = delete;
  /**
   * Returns a reference on the sample of the sensor
   */
  const ISensorValue &getSample();
  /**
   * Returns the last value sampled from the sensor expected to be in micro %RH
   */
  virtual int32_t getSensorSample() const = 0;
  void setThresholds(const ISensorThresholds &);

 protected:
  ~AHumiditySensor();

 private:
  ISensorValue _value;
  int32_t _humidity;
  int32_t _min;
  int32_t _max;
};
}  // namespace sensor
}  // namespace athome

#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_HUMIDITY_SENSOR) */
#endif /* AHUMIDITYSENSOR_HPP */
