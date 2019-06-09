#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR)
#include "ATemperatureSensor.hpp"

#define MIN_EMPTY_ROOM_TEMPERATURE 12000000
#define MAX_ROOM_TEMPERATURE 30000000

namespace athome {
namespace sensor {
ATemperatureSensor::ATemperatureSensor()
    : _value({ISensor::ISensorScale::ZERO,
              {utility::units::UNIT::DEGREE_CELSIUS,
               utility::units::PREFIX::MICRO},
              reinterpret_cast<void *>(&_temp),
              PSTR("Temperature")}),
      _temp(0),
      _min(18000000),
      _max(28000000) {}

ATemperatureSensor::~ATemperatureSensor() {}

/**
 * Return an estimation of the quality of the temperature
 *
 * Values are from:
 * https://www.jechange.fr/energie/duale/guides/thermostat-temperature-4101
 * @return estimation of temperature on a scale from 1 (worst) to 10 (best)
 */
const ISensor::ISensorValue &ATemperatureSensor::getSample() {
  _temp = getSensorSample();
  if (_temp < MIN_EMPTY_ROOM_TEMPERATURE) {
    _value.estimate = ISensor::ISensorScale::ONE;
  } else if (_temp >= MIN_EMPTY_ROOM_TEMPERATURE && _temp < _min) {
    _value.estimate = ISensor::ISensorScale::FOUR;
  } else if (_temp >= _min && _temp <= _max) {
    _value.estimate = ISensor::ISensorScale::TEN;
  } else if (_temp > _max && _temp < MAX_ROOM_TEMPERATURE) {
    _value.estimate = ISensor::ISensorScale::FOUR;
  } else {
    _value.estimate = ISensor::ISensorScale::ONE;
  }
  return _value;
}

void ATemperatureSensor::setThresholds(const ISensorThresholds &thresholds) {
  if (thresholds.unit.unit != utility::units::UNIT::DEGREE_CELSIUS) {
    return;
  }
  _min = thresholds.min;
  _max = thresholds.max;
  if (thresholds.unit.prefix == utility::units::PREFIX::BASE_UNIT) {
    _min *= 1000000;
    _max *= 1000000;
  }
}
}  // namespace sensor
}  // namespace athome
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_TEMPERATURE_SENSOR) */
