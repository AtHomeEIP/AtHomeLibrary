#ifndef AHUMIDITYSENSOR_HPP
# define AHUMIDITYSENSOR_HPP

# include "ISensor.hpp"

namespace athome {
    namespace sensor {
        class AHumiditySensor : public ISensor {
        public:
            AHumiditySensor();
            AHumiditySensor(const AHumiditySensor &) = delete;
            AHumiditySensor &operator=(const AHumiditySensor &) = delete;
            ~AHumiditySensor();
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

#endif /* AHUMIDITYSENSOR_HPP */
