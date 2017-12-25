#ifdef ARDUINO
# include <Arduino.h>
#endif /* ARDUINO */
#include "AnalogSensor.hpp"

namespace woodBox {
	namespace sensor {
		AnalogSensor::AnalogSensor(uint8_t pin):
			_analog_pin(pin),
			_last_sample(0)
		{}

		AnalogSensor::~AnalogSensor() {}

		void AnalogSensor::init() {}

		void AnalogSensor::stop() {}

		uint8_t *AnalogSensor::getSample() {
#ifdef ARDUINO
			_last_sample = analogRead(_analog_pin);
#else
			compile_error
#endif /* ARDUINO */
			return reinterpret_cast<uint8_t *>(&_last_sample);
		}

		uint8_t AnalogSensor::getAnaloguePin() {
			return _analog_pin;
		}
	}
}