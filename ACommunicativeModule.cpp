#include "ACommunicativeModule.hpp"

namespace woodBox {
    namespace module {
        ACommunicativeModule::ACommunicativeModule(Stream **streams):
            _streams(streams)
        {}

        ACommunicativeModule::ACommunicativeModule(ACommunicativeModule &other):
            _streams(other._streams)
        {
            other._streams = nullptr;
        }

        ACommunicativeModule &ACommunicativeModule::operator=(ACommunicativeModule &other) {
            _streams = other._streams;
            other._streams = nullptr;
            return *this;
        }

        ACommunicativeModule::~ACommunicativeModule() {}

        Stream **ACommunicativeModule::getStreams() {
            return _streams;
        }

        void ACommunicativeModule::setStreams(Stream **streams) {
            _streams = streams;
        }
    }
}

