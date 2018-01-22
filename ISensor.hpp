#ifndef ISENSOR_HPP
# define ISENSOR_HPP

# include <stdint.h>

namespace woodBox {
    namespace sensor {
        class ISensor {
            public:
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
                virtual uint8_t *getSample() = 0; // Get sample measure from the sensor
                virtual ISensorScale getEstimate() = 0;
        };
    }
}

#endif /* ISENSOR_HPP */
