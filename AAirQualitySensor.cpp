#include "AAirQualitySensor.hpp"

namespace athome {
    namespace sensor {
        AAirQualitySensor::AAirQualitySensor() {}

        AAirQualitySensor::~AAirQualitySensor() {}

        ISensorScale AAirQualitySensor::getEstimate() {
            return getLastSample();
        }
    }
}