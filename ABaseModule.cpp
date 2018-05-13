#include "ABaseModule.hpp"

namespace athome {
    namespace module {
        ABaseModule::ABaseModule(
#ifndef DISABLE_DISPLAY
# if !defined(DISABLE_COMMUNICATION) || !defined(DISABLE_POWER_MANAGEMENT) || !defined(DISABLE_SENSOR) ||\
     !defined(DISABLE_PERSISTENT_STORAGE)
                                 display::IDisplay *display,
# else
                                 display::IDisplay *display
# endif /* ... */
#endif /* DISABLE_DISPLAY */
#ifndef DISABLE_COMMUNICATION
# if !defined(DISABLE_POWER_MANAGEMENT) || !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                                 Stream **communicators,
# else
                                 Stream **communicators
# endif /* ... */
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_POWER_MANAGEMENT
# if !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                                 power::IPower *power,
# else
                                 power::IPower *power
# endif /* ... */
#endif /* DISABLE_POWER_MANAGEMENT */
#ifndef DISABLE_SENSOR
# if !defined(DISABLE_PERSISTENT_STORAGE)
                                 sensor::ISensor *sensor,
# else
                                 sensor::ISensor *sensor
# endif /* ... */
#endif /* DISABLE_SENSOR */
#ifndef DISABLE_PERSISTENT_STORAGE
                                 storage::IStorage *storage
#endif /* DISABLE_PERSISTENT_STORAGE */
#if !defined(DISABLE_DISPLAY) || !defined(DISABLE_COMMUNICATION) || !defined(DISABLE_POWER_MANAGEMENT) ||\
    !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                                ):
#else
                                )
#endif /* !defined(DISABLE_DISPLAY) && !defined(DISABLE_COMMUNICATION) && !defined(DISABLE_POWER_MANAGEMENT) &&\
!defined(DISABLE_SENSOR) && !defined(DISABLE_PERSISTENT_STORAGE) */
#ifndef DISABLE_DISPLAY
# if !defined(DISABLE_COMMUNICATION) || !defined(DISABLE_POWER_MANAGEMENT) ||\
     !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                                 _display(display),
# else
                                 _display(display)
# endif /* ... */
#endif /* DISABLE_DISPLAY */
#ifndef DISABLE_COMMUNICATION
# if !defined(DISABLE_POWER_MANAGEMENT) ||\
     !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                                 _streams(communicators),
# else
                                 _streams(communicators)
# endif /* ... */
#endif /* DISABLE_COMMUNICATION */
#ifndef DISABLE_POWER_MANAGEMENT
# if !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                                 _power(power),
# endif /* ... */
#endif /* DISABLE_POWER_MANAGEMENT */
#ifndef DISABLE_SENSOR
# if !defined(DISABLE_PERSISTENT_STORAGE)
                                 _sensor(sensor),
# else
                                 _sensor(sensor)
# endif /* ... */
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

