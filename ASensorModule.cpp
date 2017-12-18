#include "ASensorModule.hpp"

namespace woodBox {
	namespace module {
		ASensorModule::ASensorModule(sensor::ISensor *sensor):
			_sensor(sensor)
		{}

		ASensorModule::ASensorModule(ASensorModule &other):
			_sensor(other._sensor)
		{
			other._sensor = nullptr;
		}

		ASensorModule &ASensorModule::operator=(ASensorModule &other) {
			if (_sensor != nullptr)
				delete _sensor;
			_sensor = other._sensor;
			other._sensor = nullptr;
			return *this;
		}

		ASensorModule::~ASensorModule() {
			if (_sensor != nullptr)
				delete _sensor;
		}

		const sensor::ISensor *ASensorModule::getSensor() {
			return _sensor;
		}

		void ASensorModule::setSensor(sensor::ISensor *sensor) {
			if (_sensor != nullptr)
				delete _sensor;
			_sensor = sensor;
		}
	}
}