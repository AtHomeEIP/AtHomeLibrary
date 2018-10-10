#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR)
#ifdef ARDUINO
#include <Arduino.h>
#endif /* ARDUINO */
#include "AnalogSensor.hpp"

namespace athome {
namespace sensor {
AnalogSensor::AnalogSensor(uint8_t pin)
    : _analog_pin(pin),
      _value(
          {ISensor::ISensorScale::ZERO,
           {utility::units::UNIT::UNKNOWN, utility::units::PREFIX::BASE_UNIT},
           reinterpret_cast<void *>(&_sample),
           PSTR("Analogue sensor")}),
      _sample(0) {}

AnalogSensor::~AnalogSensor() {}

const ISensor::ISensorValue &AnalogSensor::getSample() {
#ifdef ARDUINO
  _sample = analogRead(_analog_pin);
#else
#error not implemented yet
#endif /* ARDUINO */
  return _value;
}

uint8_t AnalogSensor::getAnaloguePin() { return _analog_pin; }

void AnalogSensor::setThresholds(const ISensor::ISensorThresholds &thresholds) {
  (void)thresholds;
}
}  // namespace sensor
}  // namespace athome
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR) */