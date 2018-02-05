#ifndef ATEMPERATURESENSOR_HPP
# define ATEMPERATURESENSOR_HPP

# include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class ATemperatureSensor : public ISensor {
        public:
            ATemperatureSensor();
            ATemperatureSensor(const ATemperatureSensor &) = delete;
            ATemperatureSensor &operator=(const ATemperatureSensor &) = delete;
            ~ATemperatureSensor();
            /**
             * Returns a pointer on the sample of the sensor. This pointer needs to point on a float variable.
             */
            virtual uint8_t *getSample() = 0;
            /**
             * Returns the last value sampled from the sensor (do not actually resample it).
             */
            virtual float   getLastSample() const = 0;
            ISensorScale    getEstimate();
        };
    }
}

#endif /* ATEMPERATURESENSOR_HPP */
