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
                                 AStorageModule(storage),
                                 _scheduler(nullptr) {
            _sensorTask.setInterval(TASK_SECOND);
            _sensorTask.setIterations(TASK_FOREVER);
            _sensorTask.setCallback(&ABaseModule::_onSampleSensor);
            _displayTask.setInterval(TASK_SECOND);
            _displayTask.setIterations(TASK_FOREVER);
            _displayTask.setCallback(&ABaseModule::_onUpdateDisplay);
            _communicationTask.setInterval(TASK_MINUTE);
            _communicationTask.setIterations(TASK_FOREVER);
            _communicationTask.setCallback(&ABaseModule::_onCommunicate);
        }

        ABaseModule::ABaseModule(ABaseModule &other):
                                                    ADisplayModule(other),
                                                    ACommunicativeModule(other),
                                                    APoweredModule(other),
                                                    ASensorModule(other),
                                                    AStorageModule(other),
                                                    _scheduler(other._scheduler) {}

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

        void ABaseModule::setScheduler(Scheduler &scheduler) {
            if (_scheduler != nullptr) {
                _scheduler->deleteTask(_sensorTask);
                _scheduler->deleteTask(_displayTask);
                _scheduler->deleteTask(_communicationTask);
            }
            _scheduler = &scheduler;
            _scheduler->addTask(_sensorTask);
            _scheduler->addTask(_displayTask);
            _scheduler->addTask(_communicationTask);
        }

        Scheduler *ABaseModule::getScheduler() {
            return _scheduler;
        }

        void ABaseModule::run() {
            setup();
            //loop();
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
                /* onSampleSensor();
                onUpdateDisplay();
                onCommunicate();
                onPause();
                sleep();
                onResume(); */
                _scheduler->execute();
            }
        }

        /* void ABaseModule::sleep() {
            delay(1000);
            wakeUp();
        }

        void ABaseModule::wakeUp() {
            onResume();
        } */
    }
}

