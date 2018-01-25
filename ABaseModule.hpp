#ifndef ABASEMODULE_HPP
# define ABASEMODULE_HPP

# define _TASK_INLINE // Macro used by TaskScheduler library

# include <stdint.h>
# include <Arduino.h>
# include <TaskScheduler.h>
# include "IDisplay.hpp"
# include "IPower.hpp"
# include "ISensor.hpp"
# include "IStorage.hpp"

namespace woodBox {
    namespace module {
        /**
         * Abstract base class for all Module classes, mainly handling low level logic and accessers to Module components
         */
        class ABaseModule {
            public:
                ABaseModule &operator=(ABaseModule &) = delete; // Singleton class
                ~ABaseModule();
            protected:
                ABaseModule(display::IDisplay * = nullptr, Stream ** = nullptr,
                            power::IPower * = nullptr, sensor::ISensor * = nullptr, storage::IStorage * = nullptr);
                ABaseModule(ABaseModule &) = delete; // Singleton class
            public:
                /**
                 * Return an array of pointers on `Stream` derived objects terminated by a `nullptr`.
                 *
                 * For example, the array can be like this: `{&Serial, &Serial1, &woodBox::communication::wifi::ESP8266WiFiCommunicator, nullptr}`
                 *
                 * If there's no `Stream`s used by the module, this method returns `nullptr`
                 */
                Stream **getStreams();
                /**
                 * Set the array of pointers on Stream derived objects used by module for communications.
                 *
                 * For example: `Stream *my_array[] = {&Serial, &Serial1, &woodBox::communication::wifi::ESP8266WiFiCommunicator, nullptr};`
                 */
                void setStreams(Stream **);
                /**
                 * Return a woodBox::display::IDisplay interface pointer on the display currently used by the module.
                 *
                 * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                 *
                 * If there's no display interface currently used by the module, this method returns `nullptr`.
                 */
                display::IDisplay *getDisplay();
                /**
                 * Set the pointer on the woodBox::display::IDisplay interface used by the module.
                 */
                void setDisplay(display::IDisplay *);
                /**
                  * Return a woodBox::power::IPower interface pointer on the power supply currently used by the module.
                  *
                  * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                  *
                  * If there's no power interface currently used by the module, this method returns `nullptr`.
                  */
                power::IPower *getPowerSource();
                /**
                 * Set the pointer on the woodBox::power::IPower interface used by the module.
                 */
                void setPowerSource(power::IPower *);
                /**
                  * Return a woodBox::sensor::ISensor interface pointer on the sensor currently used by the module.
                  *
                  * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                  *
                  * If there's no sensor interface currently used by the module, this method returns `nullptr`.
                  */
                sensor::ISensor *getSensor();
                /**
                 * Set the pointer on the woodBox::sensor::ISensor interface used by the module.
                 */
                void setSensor(sensor::ISensor *);
                /**
                  * Return a woodBox::storage::IStorage interface pointer on the storage currently used by the module.
                  *
                  * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                  *
                  * If there's no storage interface currently used by the module, this method returns `nullptr`.
                  */
                storage::IStorage *getStorage();
                /**
                 * Set the pointer on the woodBox::storage::IStorage interface used by the module.
                 */
                void setStorage(storage::IStorage *);
            protected:
                display::IDisplay   *_display;
                Stream              **_streams;
                power::IPower       *_power;
                sensor::ISensor     *_sensor;
                storage::IStorage   *_storage;
        };
    }
}

#endif /* ABASEMODULE_HPP */
