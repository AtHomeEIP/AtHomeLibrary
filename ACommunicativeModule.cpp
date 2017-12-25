#include "ACommunicativeModule.hpp"

namespace woodBox {
    namespace module {
        ACommunicativeModule::ACommunicativeModule(Stream *stream):
            _stream(stream)
        {}

        ACommunicativeModule::ACommunicativeModule(ACommunicativeModule &other):
            _stream(other._stream)
        {
            other._stream = nullptr;
        }

        ACommunicativeModule &ACommunicativeModule::operator=(ACommunicativeModule &other) {
			if (_stream != nullptr)
				delete _stream;
            _stream = other._stream;
            other._stream = nullptr;
            return *this;
        }

        ACommunicativeModule::~ACommunicativeModule() {
			if (_stream != nullptr)
				delete _stream;
        }

        const Stream *ACommunicativeModule::getCommunicator() {
            return _stream;
        }

        void ACommunicativeModule::setCommunicator(Stream *stream) {
			if (_stream != nullptr)
				delete _stream;
            _stream = stream;
        }
    }
}

