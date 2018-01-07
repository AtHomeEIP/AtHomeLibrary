#include "ABaseModule.hpp"

#ifndef ARDUINO
inline void delay(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (uint32_t j = 0; j < 33; j++);
    }
}
#endif /* ARDUINO */

namespace woodBox {
    namespace module {
        ABaseModule *ABaseModule::_instance = nullptr;

        ABaseModule::ABaseModule(
                                 display::IDisplay *display,
                                 communication::ICommunicator **communicators,
                                 power::IPower *power,
                                 sensor::ISensor *sensor,
                                 storage::IStorage *storage
                                ):
                                 ADisplayModule(display),
                                 ACommunicativeModule(communicators),
                                 APoweredModule(power),
                                 ASensorModule(sensor),
                                 AStorageModule(storage) {}

        ABaseModule::ABaseModule(ABaseModule &other):
                                                    ADisplayModule(other),
                                                    ACommunicativeModule(other),
                                                    APoweredModule(other),
                                                    ASensorModule(other),
                                                    AStorageModule(other) {}

        ABaseModule &ABaseModule::operator=(ABaseModule &other) {
            /* DisplayModule::operator=(other);
            CommunicativeModule::operator=(other);
            PoweredModule::operator=(other);
            SensorModule::operator=(other);
            StorageModule::operator=(other); */
            if (_instance != nullptr && _instance != this) {
                delete _instance;
                _instance = this;
            }
            return *this;
        }

        ABaseModule::~ABaseModule() {}

        void ABaseModule::run() {
            setup();
            loop();
        }

        void ABaseModule::stop() {
            onBackupOnStorage();
            onStop();
        }

        void ABaseModule::setup() {
            onRestoreFromStorage();
            onStart();
        }

        void ABaseModule::loop() {
            while (1) {
                onSampleSensor();
                onUpdateDisplay();
                onCommunicate();
                onPause();
                sleep();
                onResume();
            }
        }

        void ABaseModule::sleep() {
            delay(1000);
            wakeUp();
        }

        void ABaseModule::wakeUp() {
            onResume();
        }
    }
}

