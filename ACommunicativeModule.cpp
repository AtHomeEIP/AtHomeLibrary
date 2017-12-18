#include "ACommunicativeModule.hpp"

namespace woodBox {
    namespace module {
        ACommunicativeModule::ACommunicativeModule(communication::ICommunicator *communicator):
            _communicator(communicator)
        {}

        ACommunicativeModule::ACommunicativeModule(ACommunicativeModule &other):
            _communicator(other._communicator)
        {
            other._communicator = nullptr;
        }

        ACommunicativeModule &ACommunicativeModule::operator=(ACommunicativeModule &other) {
			if (_communicator != nullptr)
				delete _communicator;
            _communicator = other._communicator;
            other._communicator = nullptr;
            return *this;
        }

        ACommunicativeModule::~ACommunicativeModule() {
			if (_communicator != nullptr)
				delete _communicator;
        }

        const communication::ICommunicator *ACommunicativeModule::getCommunicator() {
            return _communicator;
        }

        void ACommunicativeModule::setCommunicator(communication::ICommunicator *communicator) {
			if (_communicator != nullptr)
				delete _communicator;
            _communicator = communicator;
        }
    }
}

