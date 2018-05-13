#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR)
# if defined(ARDUINO) && (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__) ||\
     defined(__AVR_ATmega2560__))
# include <Wire.h>
# include <Digital_Light_TSL2561.h>
# include "GroveDigitalLightSensor.hpp"

namespace athome {
    namespace sensor {
        GroveDigitalLightSensor::GroveDigitalLightSensor() {
            Wire.begin();
            TSL2561.init();
        }

        GroveDigitalLightSensor::~GroveDigitalLightSensor() {}

        GroveDigitalLightSensor *GroveDigitalLightSensor::getInstance() {
            if (_instance == nullptr) {
                _instance = new GroveDigitalLightSensor();
            }
            return _instance;
        }

        uint8_t *GroveDigitalLightSensor::getSample() {
            _sample = TSL2561.readVisibleLux();
            return reinterpret_cast<uint8_t *>(&_sample);
        }

        uint16_t GroveDigitalLightSensor::getLastSample() const {
            return _sample;
        }

        GroveDigitalLightSensor *GroveDigitalLightSensor::_instance = nullptr;
    }
}
# endif /* ARDUINO */
#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) && !defined(DISABLE_GROVE_DIGITAL_LIGHT_SENSOR) */