#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && \
    !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR)
#if defined(ARDUINO) &&                                           \
    (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || \
     defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega2560__))
#include <Arduino.h>
#include <Digital_Light_TSL2561.h>
#include <Wire.h>
#include "GroveDigitalLightSensor.hpp"

namespace athome {
namespace sensor {
GroveDigitalLightSensor::GroveDigitalLightSensor() {
  Wire.begin();
  TSL2561.init();
}

GroveDigitalLightSensor::~GroveDigitalLightSensor() {}

uint16_t GroveDigitalLightSensor::getSensorSample() {
  return TSL2561.readVisibleLux();
}
}  // namespace sensor
}  // namespace athome
#endif /* ARDUINO */
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && \
          !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR) */