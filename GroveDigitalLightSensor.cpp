#ifdef ARDUINO
# include "GroveDigitalLightSensor.hpp"

namespace athome {
    namespace sensor {
        GroveDigitalLightSensor::GroveDigitalLightSensor() {
            Wire.begin();
            TSL2561.init();
        }

        GroveDigitalLightSensor::~GroveDigitalLightSensor() {}

        GroveDigitalLightSensor &GroveDigitalLightSensor::getInstance() {
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
    }
}
#endif /* ARDUINO */