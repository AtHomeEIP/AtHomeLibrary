#include "ABaseModule.hpp"

namespace athome {
    namespace module {
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
                                 _storage(storage) {
        }

        ABaseModule::~ABaseModule() {}

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

        power::IPower *ABaseModule::getPowerSource() {
            return _power;
        }

        void ABaseModule::setPowerSource(power::IPower *power) {
            _power = power;
        }

        sensor::ISensor *ABaseModule::getSensor() {
            return _sensor;
        }

        void ABaseModule::setSensor(sensor::ISensor *sensor) {
            _sensor = sensor;
        }

        storage::IStorage *ABaseModule::getStorage() {
            return _storage;
        }

        void ABaseModule::setStorage(storage::IStorage *storage) {
            _storage = storage;
        }
    }
}

