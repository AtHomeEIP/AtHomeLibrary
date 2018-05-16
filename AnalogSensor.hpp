#ifndef ANALOG_SENSOR_HPP
# define ANALOG_SENSOR_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR)
#  include <stdint.h>
#  include "ISensor.hpp"

namespace athome {
	namespace sensor {
	    /**
	     * Base class derived from athome::sensor::ISensor allowing to use an analog sensor.
	     *
	     * Warning: This class is only able to sample an analog sensor, not to interpret its value.
	     *
	     * Thus, `getEstimate` method will always return athome::sensor::ISensor::ISensorScale::ZERO if this method is not overriden by a specialized class.
	     */
		class AnalogSensor : public ISensor {
			public:
		        /**
		         * Create an instance of AnalogSensor, needing the analog pin used to sample the sensor.
		         */
				AnalogSensor(uint8_t pin);
				AnalogSensor(const AnalogSensor &) = delete;
				AnalogSensor &operator=(const AnalogSensor &) = delete;
				~AnalogSensor();
				/**
				 * Return a pointer on a `uint16_t` variable, so the memory area returned by the pointer is 2 bytes long or equivalent to an array of 2 `uint8_t`.
				 *
				 * Examples of use:
				 *
				 * \code{.cpp}
				 * void my_func_reading_my_sensor(ISensor &my_sensor) {
				 *   uint8_t *pointer = my_sensor.getSample();
				 *   if (pointer == nullptr) {
				 *     // Invalid pointer -> might be caused by a sample error from the sensor.
				 *     // Can't happen with base AnalogSensor class but could happen from other athome::sensor::ISensor derived classes, so it's good practice to always check returned pointer.
				 *     return;
				 *   }
				 *   // Can access value from casting pointer to its original uint16_t type:
				 *   uint16_t *full_pointer = reinterpret_cast<uint16_t *>(pointer);
				 *   // Or by casting it in C-style:
				 *   uint16_t *c_pointer = (uint16_t *)pointer;
				 *   // Or by accessing it as an array of 2 bytes:
				 *   uint16_t value = ((pointer[0]) << 8) | pointer[1];
				 *
				 *   printf("%hu\n", *full_pointer); // Or Serial.println(*full_pointer) for example on Arduino
				 *   printf("%hu\n", *c_pointer); // Or Serial.println(*c_pointer) for example on Arduino
				 *   printf("%hu\n", value); // Or Serial.println(value) for example on Arduino
				 * }
				 * \endcode
				 */
				const ISensorValue &getSample();
				void setThresholds(const ISensorThresholds &);
                /**
                 * Return the value of the pin used to sample the sensor.
                 */
                uint8_t getAnaloguePin();
			protected:
				uint8_t			_analog_pin;
			private:
				ISensorValue	_value;
				uint16_t		_sample;
		};
	}
}

# endif /* !defined(DISABLE_SENSOR) && !defined(DISABLE_ANALOG_SENSOR) */
#endif /* ANALOG_SENSOR_HPP */
