#include "APoweredModule.hpp"

namespace woodBox {
	namespace module {
		APoweredModule::APoweredModule(power::IPower *power):
			_power(power)
		{}

		APoweredModule::APoweredModule(APoweredModule &other):
			_power(other._power)
		{
			other._power = nullptr;
		}

		APoweredModule &APoweredModule::operator=(APoweredModule &other) {
			if (_power != nullptr)
				delete _power;
			_power = other._power;
			other._power = nullptr;
			return *this;
		}

		APoweredModule::~APoweredModule() {
			if (_power != nullptr)
				delete _power;
		}

		const power::IPower *APoweredModule::getPowerSource() {
			return _power;
		}

		void APoweredModule::setPowerSource(power::IPower *power) {
			if (_power != nullptr)
				delete _power;
			_power = power;
		}
	}
}