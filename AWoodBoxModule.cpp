#include "AWoodBoxModule.hpp"

namespace woodBox {
    namespace module {
        AWoodBoxModule::AWoodBoxModule():ABaseModule() {}

        AWoodBoxModule::~AWoodBoxModule() {}

        void AWoodBoxModule::onBackupOnStorage() {
            // TODO: to implement
        }

        void AWoodBoxModule::onRestoreFromStorage() {
            // TODO: to implement
        }

        void AWoodBoxModule::onCommunicate() {
            if (_streams == nullptr) {
                return;
            }
            unsigned long duration = millis() + _comDuration;
            while (millis() < duration) {
                for (communication::ICommunicator *com = *_streams; com != nullptr; com++) {

                }
            }
        }
    }
}
