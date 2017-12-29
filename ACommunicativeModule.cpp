#include "ACommunicativeModule.hpp"

namespace woodBox {
    namespace module {
        ACommunicativeModule::ACommunicativeModule(communication::ICommunicator *stream):
            _stream(stream)
        {
			stream->open();
		}

        ACommunicativeModule::ACommunicativeModule(ACommunicativeModule &other):
            _stream(other._stream)
        {
            other._stream = nullptr;
        }

        ACommunicativeModule &ACommunicativeModule::operator=(ACommunicativeModule &other) {
			if (_stream != nullptr)
				_stream->close();
            _stream = other._stream;
            other._stream = nullptr;
            return *this;
        }

        ACommunicativeModule::~ACommunicativeModule() {
			if (_stream != nullptr)
				_stream->close();
        }

        const communication::ICommunicator *ACommunicativeModule::getCommunicator() {
            return _stream;
        }

        void ACommunicativeModule::setCommunicator(communication::ICommunicator *stream) {
			if (_stream != nullptr)
				_stream->close();
            _stream = stream;
        }
    }
}

