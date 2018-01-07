#ifndef ISENSOR_HPP
# define ISENSOR_HPP

# include <stdint.h>

namespace woodBox {
    namespace sensor {
        class ISensor {
            public:
                //virtual ~ISensor() = 0;
                virtual void init() = 0; // Initialize the sensor if needed, either warmup of the sensor or used communication interface such as I2C
                virtual void stop() = 0; // Stop the sensor
                virtual uint8_t *getSample() = 0; // Get sample measure from the sensor
        };
    }
}

#endif /* ISENSOR_HPP */
