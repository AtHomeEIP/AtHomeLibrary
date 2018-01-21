#include "ABaseModule.hpp"

namespace woodBox {
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

        /* ABaseModule::ABaseModule(ABaseModule &other) {
            (void*)other;
        }

        ABaseModule &ABaseModule::operator=(ABaseModule &other) {
            (void*)other;
            return *this;
        } */

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

