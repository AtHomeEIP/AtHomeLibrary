#ifndef ISENSOR_HPP
# define ISENSOR_HPP

# include <stdint.h>

namespace woodBox {
    namespace sensor {
        /**
         * Interface used to grab data from a sensor in a Module class.
         *
         * Configuration and treatment of sensor raw data is to be handled by derived classes,
         * as this interfaces is intended to be able to just being able to grab raw sensors data,
         * and an interpretation of them without any knowledge of the sensor.
         */
        class ISensor {
            public:
                /**
                 * Enumeration used to represent the estimation of safety of a sensor value on a scale from 1 to 10. Value 0 means invalid
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
                //virtual ~ISensor() = 0;
                /**
                 * Returns a pointer on sensor sample raw memory, as an array of bytes
                 */
                virtual uint8_t *getSample() = 0;
                /**
                 * Returns the estimation of safety from the current sensor value
                 */
                virtual ISensorScale getEstimate() = 0;
        };
    }
}

#endif /* ISENSOR_HPP */
