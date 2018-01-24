#ifndef ANALOG_SENSOR_HPP
# define ANALOG_SENSOR_HPP

# include <stdint.h>
# include "ISensor.hpp"

namespace woodBox {
	namespace sensor {
		class AnalogSensor : public ISensor {
			public:
				AnalogSensor(uint8_t pin);
				AnalogSensor(const AnalogSensor &) = delete;
				AnalogSensor &operator=(const AnalogSensor &) = delete;
				~AnalogSensor();
				virtual uint8_t *getSample();
                      virtual ISensorScale getEstimate();				uint8_t getAnaloguePin();
			protected:
				uint8_t		_analog_pin;
			private:
				uint16_t	_last_sample;
		};
	}
}

#endif /* ANALOG_SENSOR_HPP */