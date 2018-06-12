#include "AtHomeConfig.h"
#if !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR)
# include "AtHomeFlashCommon.h"
# include "ALuminositySensor.hpp"

namespace athome {
    namespace sensor {
        namespace {
            static const PROGMEM uint16_t minimalLuxValues[ALuminositySensor::Room_Type::ARCHIVES + 1] = {
                    0,
                    100,
                    150,
                    100,
                    300,
                    500,
                    100,
                    300,
                    300,
                    500,
                    500,
                    500,
                    750,
                    500,
                    200
            };
        }

        ALuminositySensor::ALuminositySensor():_value({
                                                              ISensor::ISensorScale::ZERO,
                                                              {
                                                                      utility::units::UNIT::LUX,
                                                                      utility::units::PREFIX::BASE_UNIT
                                                              },
                                                              reinterpret_cast<void *>(&_lux),
                                                              PSTR("Luminosity")
                                                      }),
                                               _lux(0),
                                               _min(500),
                                               _max(30000)
        {}

        ALuminositySensor::~ALuminositySensor() {}

        /**
         * Return an estimation of the luminosity quality in the room
         *
         * Values are from french norm NF EN 12464-1 https://fr.wikipedia.org/wiki/Lux_%28unit%C3%A9%29
         * @return estimation of the luminosity on a scale from 1 (worst) to 10 (best)
         */
        const ISensor::ISensorValue& ALuminositySensor::getSample() {
            _lux = getSensorSample();
            _value.estimate = (_lux < _min || _lux > _max) ? ISensor::ISensorScale::ONE : ISensor::ISensorScale::TEN;
            return _value;
        }

        void ALuminositySensor::setThresholds(const ISensor::ISensorThresholds &thresholds) {
            if (thresholds.unit.unit != utility::units::UNIT::LUX) {
                return;
            }
            _min = thresholds.min;
            _max = thresholds.max;
        }

        void ALuminositySensor::setRoom(ALuminositySensor::Room_Type room) {
            _min = pgm_read_word(&minimalLuxValues[room]);
        }
    }
}

#endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_LUMINOSITY_SENSOR) */