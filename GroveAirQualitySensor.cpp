#if defined(AVR_PRO) || defined(AVR_UNO_WIFI_DEV_ED) || defined(AVR_NG) || \
        defined(AVR_LILYPAD) || defined(AVR_BT) || defined(AVR_FIO) || \
        defined(AVR_ETHERNET) || defined(AVR_MINI) || defined(AVR_NANO) || \
        defined(AVR_DUEMILANOVE) || defined(AVR_UNO)

# include "GroveAirQualitySensor.hpp"

namespace woodBox {
    namespace sensor {
        GroveAirQualitySensor::GroveAirQualitySensor(uint8_t pin):
                _lastMeasure(0) {
            _sensor.init(pin);
        }

        GroveAirQualitySensor::~GroveAirQualitySensor() {}

        uint8_t *GroveAirQualitySensor::getSample() {
            _sensor.last_vol = _sensor.first_vol;
            _sensor.first_vol = analogRead(A0);
            _sensor.counter = 0;
            _sensor.timer_index = 1;
            _lastMeasure = _sensor.slope();
            return &_lastMeasure;
        }

        ISensor::ISensorScale GroveAirQualitySensor::getEstimate() {
            switch (_lastMeasure) {
            case 0:
                return ISensor::ISensorScale::ONE;
            case 1:
                return ISensor::ISensorScale::THREE;
            case 2:
                return ISensor::ISensorScale::FIVE;
            case 3:
                return ISensor::ISensorScale::TEN;
            default:
                return ISensor::ISensorScale::ZERO;
            }
        }
    }
}

#endif /* __AVR__ */
