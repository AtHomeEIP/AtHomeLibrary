#include "ACommunicativeModule.hpp"

namespace woodBox {
    namespace module {
        ACommunicativeModule::ACommunicativeModule(communication::ICommunicator **streams):
            _streams(streams)
        {
            if (_streams != nullptr) {
                for (communication::ICommunicator *stream = _streams[0]; stream != nullptr; stream++) {
                    stream->open();
                }
            }
        }

        ACommunicativeModule::ACommunicativeModule(ACommunicativeModule &other):
            _streams(other._streams)
        {
            other._streams = nullptr;
        }

        ACommunicativeModule &ACommunicativeModule::operator=(ACommunicativeModule &other) {
            if (_streams != nullptr) {
                for (communication::ICommunicator *stream = _streams[0]; stream != nullptr; stream++) {
                    stream->close();
                }
            }
            _streams = other._streams;
            other._streams = nullptr;
            return *this;
        }

        ACommunicativeModule::~ACommunicativeModule() {
            if (_streams != nullptr) {
                for (communication::ICommunicator *stream = _streams[0]; stream != nullptr; stream++) {
                    stream->close();
                }
            }
        }

        communication::ICommunicator **ACommunicativeModule::getCommunicators() {
            return _streams;
        }

        void ACommunicativeModule::setCommunicators(communication::ICommunicator **streams) {
            if (_streams != nullptr) {
                for (communication::ICommunicator *stream = _streams[0]; stream != nullptr; stream++) {
                    stream->close();
                }
            }
            _streams = streams;
        }
    }
}

