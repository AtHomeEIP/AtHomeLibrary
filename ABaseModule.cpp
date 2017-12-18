#include "ABaseModule.hpp"

namespace woodBox {
    namespace module {
		ABaseModule *ABaseModule::_instance = nullptr;

        ABaseModule::ABaseModule(
			display::IDisplay *display,
			communication::ICommunicator *communicator,
			power::IPower *power,
			sensor::ISensor *sensor,
			storage::IStorage *storage):
            ADisplayModule(display),
            ACommunicativeModule(communicator),
			APoweredModule(power),
			ASensorModule(sensor),
			AStorageModule(storage)
        {
			onStart();
		}

        ABaseModule::ABaseModule(ABaseModule &other):
            ADisplayModule(other),
            ACommunicativeModule(other),
			APoweredModule(other),
			ASensorModule(other),
			AStorageModule(other)
        {}

        /* ABaseModule &ABaseModule::operator=(ABaseModule &other) {
            DisplayModule::operator=(other);
            CommunicativeModule::operator=(other);
			PoweredModule::operator=(other);
			SensorModule::operator=(other);
			StorageModule::operator=(other);
			if (_instance != nullptr && _instance != this) {
				delete _instance;
				_instance = this;
			}
            return *this;
        } */

        ABaseModule::~ABaseModule() {}

        void ABaseModule::run() {
        }

        void ABaseModule::stop() {
            onStop();
        }
    }
}

