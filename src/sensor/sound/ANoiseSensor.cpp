#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_NOISE_SENSOR)
#include "ANoiseSensor.hpp"

namespace athome {
namespace sensor {
ANoiseSensor::ANoiseSensor()
    : _value(
          {ISensor::ISensorScale::ZERO,
           {utility::units::UNIT::UNKNOWN, utility::units::PREFIX::BASE_UNIT},
           reinterpret_cast<void *>(&_noise),
           PSTR("Noise Sensor")}),
      _noise(0),
      _min(0),
      _max(0) {}

ANoiseSensor::~ANoiseSensor() {}

const ISensor::ISensorValue &ANoiseSensor::getSample() { return _value; }

void ANoiseSensor::setThresholds(const ISensor::ISensorThresholds &thresholds) {
  (void)thresholds;
}
}  // namespace sensor
}  // namespace athome

#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR) */