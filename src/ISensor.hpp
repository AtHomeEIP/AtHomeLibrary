#ifndef ISENSOR_HPP
#define ISENSOR_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR)
#include <stdint.h>
#include "AtHomeFlashCommon.h"
#include "AtHomeUnits.hpp"

namespace athome {
namespace sensor {
/**
 * Interface used to grab data from a sensor in a Module class.
 *
 * Configuration and treatment of sensor raw data is to be handled by derived
 * classes, as this interfaces is intended to be able to just being able to grab
 * raw sensors data, and an interpretation of them without any knowledge of the
 * sensor.
 */
class ISensor {
 public:
  /**
   * Enumeration used to represent the estimation of safety of a sensor value on
   * a scale from 1 to 10. Value 0 means invalid
   */
  enum ISensorScale {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN
  };
  struct ISensorValue {
    ISensorScale estimate;
    utility::units::Unit unit;
    void *sampleRawPointer;
    PGM_P label;
  };
  struct ISensorThresholds {
    utility::units::Unit unit;
    uint32_t min;
    uint32_t max;
  };
  /**
   * Returns a pointer on sensor sample raw memory, as an array of bytes
   */
  virtual const ISensorValue &getSample() = 0;
  /**
   * Returns the estimation of safety from the current sensor value
   *
   * Example:
   *
   * \code{.cpp}
   * void my_function_telling_if_a_sensor_value_is_good_or_not(ISensor
   * &my_sensor) { ISensorScale sensor_estimate = my_sensor.getEstimate(); if
   * (sensor_estimate == ISensor::ZERO) { Serial.println("The sensor returned an
   * invalid value");
   *   }
   *   else if (sensor_estimate > ISensor::ZERO && sensor_estimate < 6) {
   *     Serial.println("Boouh, it's not good :(");
   *   }
   *   else {
   *     Serial.println("Yaaaay!");
   *   }
   * }
   * \endcode
   */
  virtual void setThresholds(const ISensorThresholds &) = 0;
 protected:
  ~ISensor() {}
};
}  // namespace sensor
}  // namespace athome

#endif /* DISABLE_SENSOR */
#endif /* ISENSOR_HPP */
