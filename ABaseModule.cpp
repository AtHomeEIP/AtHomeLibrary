#include "ABaseModule.hpp"

namespace woodBox {
    namespace module {
        ABaseModule *ABaseModule::_instance = nullptr;

        ABaseModule::ABaseModule(
                                 display::IDisplay *display,
                                 Stream **communicators,
                                 power::IPower *power,
                                 sensor::ISensor *sensor,
                                 storage::IStorage *storage
                                ):
                                 _display(display),
                                 _streams(communicators),
                                 _power(power),
                                 _sensor(sensor),
                                 _storage(storage),
                                 _scheduler(nullptr),
                                 _sensorInterval(TASK_SECOND),
                                 _displayInterval(TASK_SECOND),
                                 _communicationInterval(TASK_MILLISECOND) {
            _sensorTask.setInterval(_sensorInterval);
            _sensorTask.setIterations(TASK_FOREVER);
            _sensorTask.setCallback(&ABaseModule::_onSampleSensor);
            _displayTask.setInterval(_displayInterval);
            _displayTask.setIterations(TASK_FOREVER);
            _displayTask.setCallback(&ABaseModule::_onUpdateDisplay);
            _communicationTask.setInterval(_communicationInterval);
            _communicationTask.setIterations(TASK_FOREVER);
            _communicationTask.setCallback(&ABaseModule::_onCommunicate);
        }

        /* ABaseModule::ABaseModule(ABaseModule &other) {
            (void*)other;
        }

        ABaseModule &ABaseModule::operator=(ABaseModule &other) {
            (void*)other;
            return *this;
        } */

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
            loop();
        }

        void ABaseModule::stop() {
            onBackupOnStorage();
            onStop();
        }

        void ABaseModule::setup() {
            onRestoreFromStorage();
            onStart();
            _sensorTask.enableIfNot();
            _displayTask.enableIfNot();
            _communicationTask.enableIfNot();
        }

        void ABaseModule::loop() {
            _scheduler->execute();
        }

        void ABaseModule::setSensorExecutionInterval(unsigned long ms) {
            if (ms && ms != _sensorInterval) {
                _sensorInterval = ms;
                _sensorTask.setInterval(_sensorInterval * TASK_MILLISECOND);
            }
        }

        void ABaseModule::setDisplayExecutionInterval(unsigned long ms) {
            if (ms && ms != _displayInterval) {
                _displayInterval = ms;
                _displayTask.setInterval(_displayInterval * TASK_MILLISECOND);
            }
        }

        void ABaseModule::setCommunicationExecutionInterval(unsigned long ms) {
            if (ms && ms != _communicationInterval) {
                _communicationInterval = ms;
                _communicationTask.setInterval(_communicationInterval * TASK_MILLISECOND);
            }
        }

        unsigned long ABaseModule::getSensorExecutionInterval() const {
            return _sensorInterval;
        }

        unsigned long ABaseModule::getDisplayExecutionInterval() const {
            return _displayInterval;
        }

        unsigned long ABaseModule::getCommunicationExecutionInterval() const {
            return _communicationInterval;
        }

        void ABaseModule::setSensorExecutionCallback(customCallback f) {
            _sensorTask.setCallback((f == nullptr) ? &ABaseModule::_onSampleSensor : f);
        }

        void ABaseModule::setDisplayExecutionCallback(customCallback f) {
            _displayTask.setCallback((f == nullptr) ? &ABaseModule::_onUpdateDisplay : f);
        }

        void ABaseModule::setCommunicationExecutionCallback(customCallback f) {
            _communicationTask.setCallback((f == nullptr) ? &ABaseModule::_onCommunicate : f);
        }

        Stream **ABaseModule::getStreams() {
            return _streams;
        }

        void ABaseModule::setStreams(Stream **streams) {
            _streams = streams;
        }

        display::IDisplay *ABaseModule::getDisplay() {
            return _display;
        }

        void ABaseModule::setDisplay(display::IDisplay *display) {
            _display = display;
        }

        const power::IPower *ABaseModule::getPowerSource() const {
            return _power;
        }

        void ABaseModule::setPowerSource(power::IPower *power) {
            _power = power;
        }

        const sensor::ISensor *ABaseModule::getSensor() const {
            return _sensor;
        }

        void ABaseModule::setSensor(sensor::ISensor *sensor) {
            _sensor = sensor;
        }

        const storage::IStorage *ABaseModule::getStorage() const {
            return _storage;
        }

        void ABaseModule::setStorage(storage::IStorage *storage) {
            _storage = storage;
        }
    }
}

