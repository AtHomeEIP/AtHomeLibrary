#include "ABaseModule.hpp"

namespace athome {
    namespace module {
        ABaseModule::ABaseModule(
#ifndef DISABLE_DISPLAY
                                 display::IDisplay *display,
#endif /* DISABLE_DISPLAY */
#ifndef DISABLE_COMMUNICATION
                                 Stream **communicators,
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_POWER_MANAGEMENT
                                 power::IPower *power,
#endif /* DISABLE_POWER_MANAGEMENT */
#ifndef DISABLE_SENSOR
                                 sensor::ISensor *sensor,
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_PERSISTENT_STORAGE
                                 storage::IStorage *storage
#endif /* DISABLE_PERSISTENT_STORAGE */
                                ):
#ifndef DISABLE_DISPLAY
                                 _display(display),
#endif /* DISABLE_DISPLAY */
#ifndef DISABLE_COMMUNICATION
                                 _streams(communicators),
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_POWER_MANAGEMENT
                                 _power(power),
#endif /* DISABLE_POWER_MANAGEMENT */
#ifndef DISABLE_SENSOR
                                 _sensor(sensor),
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_PERSISTENT_STORAGE
                                 _storage(storage)
#endif /* DISABLE_PERSISTENT_STORAGE */
        {
        }

        ABaseModule::~ABaseModule() {}

#ifndef DISABLE_COMMUNICATION
        Stream **ABaseModule::getStreams() {
            return _streams;
        }

        void ABaseModule::setStreams(Stream **streams) {
            _streams = streams;
        }
#endif /* DISABLE_COMMUNICATION */

#ifndef DISABLE_DISPLAY
        display::IDisplay *ABaseModule::getDisplay() {
            return _display;
        }

        void ABaseModule::setDisplay(display::IDisplay *display) {
            _display = display;
        }
#endif /* DISABLE_DISPLAY */

#ifndef DISABLE_POWER_MANAGEMENT
        power::IPower *ABaseModule::getPowerSource() {
            return _power;
        }

        void ABaseModule::setPowerSource(power::IPower *power) {
            _power = power;
        }
#endif /* DISABLE_POWER_MANAGEMENT */

#ifndef DISABLE_SENSOR
        sensor::ISensor *ABaseModule::getSensor() {
            return _sensor;
        }

        void ABaseModule::setSensor(sensor::ISensor *sensor) {
            _sensor = sensor;
        }
#endif /* DISABLE_SENSOR */

#ifndef DISABLE_PERSISTENT_STORAGE
        storage::IStorage *ABaseModule::getStorage() {
            return _storage;
        }

        void ABaseModule::setStorage(storage::IStorage *storage) {
            _storage = storage;
        }
#endif /* DISABLE_PERSISTENT_STORAGE */
    }
}

