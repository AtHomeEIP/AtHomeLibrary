#include "AAirQualitySensor.hpp"

namespace athome {
    namespace sensor {
        AAirQualitySensor::AAirQualitySensor() {}

        AAirQualitySensor::~AAirQualitySensor() {}

        ISensor::ISensorScale AAirQualitySensor::getEstimate() {
            return getLastSample();
        }
    }
}