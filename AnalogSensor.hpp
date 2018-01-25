#ifndef ANALOG_SENSOR_HPP
# define ANALOG_SENSOR_HPP

# include <stdint.h>
# include "ISensor.hpp"

namespace woodBox {
	namespace sensor {
	    /**
	     * Base class derived from woodBox::sensor::ISensor allowing to use an analog sensor.
	     *
	     * Warning: This class is only able to sample an analog sensor, not to interpret its value.
	     *
	     * Thus, `getEstimate` method will always return woodBox::sensor::ISensor::ISensorScale::ZERO if this method is not overriden by a specialized class.
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
				 * AnalogSensor my_sensor(0); // Or A0 for example if using Arduino
				 *
				 * void my_func_reading_my_sensor() {
				 *   uint8_t *pointer = my_sensor.getSample();
				 *   if (pointer == nullptr) {
				 *     // Invalid pointer -> might be caused by a sample error from the sensor.
				 *     // Can't happen with base AnalogSensor class but could happen from other woodBox::sensor::ISensor derived classes, so it's good practice to always check returned pointer.
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
				virtual uint8_t *getSample();
				/**
				 * Return woodBox::sensor::ISensor::ISensorScale::ZERO if object is an instance of AnalogSensor class
				 */
                virtual ISensorScale getEstimate();
                /**
                 * Return the value of the pin used to sample the sensor.
                 */
                uint8_t getAnaloguePin();
			protected:
				uint8_t		_analog_pin;
			private:
				uint16_t	_last_sample;
		};
	}
}

#endif /* ANALOG_SENSOR_HPP */
