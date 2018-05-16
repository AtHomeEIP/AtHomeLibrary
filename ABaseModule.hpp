#ifndef ABASEMODULE_HPP
# define ABASEMODULE_HPP

# include "AtHomeConfig.h"
# include <Arduino.h>
# include <stdint.h>
# include "IDisplay.hpp"
# include "IPower.hpp"
# include "ISensor.hpp"
# include "IStorage.hpp"

namespace athome {
    namespace module {
        /**
         * Abstract base class for all Module classes, mainly handling low level logic and accessers to Module components
         */
        class ABaseModule {
            public:
                ABaseModule &operator=(ABaseModule &) = delete; // Singleton class
                ~ABaseModule();
            protected:
                ABaseModule(
# ifndef DISABLE_DISPLAY
#  if !defined(DISABLE_COMMUNICATION) || !defined(DISABLE_POWER_MANAGEMENT) ||\
      !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                            display::IDisplay * = nullptr,
#  else
                            display::IDisplay * = nullptr
#  endif /* ... */
# endif /* DISABLE_DISPLAY */
# ifndef DISABLE_COMMUNICATION
#  if !defined(DISABLE_POWER_MANAGEMENT) || !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                            Stream ** = nullptr,
#  else
                            Stream ** = nullptr
#  endif /* ... */
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_POWER_MANAGEMENT
#  if !defined(DISABLE_SENSOR) || !defined(DISABLE_PERSISTENT_STORAGE)
                            power::IPower * = nullptr,
#  else
                            power::IPower * = nullptr
#  endif /* ... */
# endif /* DISABLE_POWER_MANAGEMENT */
# ifndef DISABLE_SENSOR
#  if !defined(DISABLE_PERSISTENT_STORAGE)
                            sensor::ISensor * = nullptr,
#  else
                            sensor::ISensor * = nullptr
#  endif /* ... */
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_PERSISTENT_STORAGE
                            storage::IStorage * = nullptr
# endif /* DISABLE_PERSISTENT_STORAGE */
                           );
                ABaseModule(ABaseModule &) = delete; // Singleton class
            public:
# ifndef DISABLE_COMMUNICATION
                /**
                 * Return an array of pointers on `Stream` derived objects terminated by a `nullptr`.
                 *
                 * For example, the array can be like this: `{&Serial, &Serial1, &athome::communication::wifi::ESP8266WiFiCommunicator, nullptr}`
                 *
                 * If there's no `Stream`s used by the module, this method returns `nullptr`
                 */
                Stream **getStreams();
                /**
                 * Set the array of pointers on Stream derived objects used by module for communications.
                 *
                 * \param   streams NULL terminated array of pointers on Stream instances
                 *
                 * For example: `Stream *my_array[] = {&Serial, &Serial1, &athome::communication::wifi::ESP8266WiFiCommunicator, nullptr};`
                 *
                 * Example on Arduino with the implementation athome::module::AtHomeModule of this class:
                 *
                 * \code{.cpp}
                 * #include <AtHome.h>
                 *
                 * AtHomeModule<uint16_t, 15> *module = AtHomeModule::<uint16_t, 15>::getInstance(); // Create a AtHomeModule instance able to buffer 15 measures from an analog sensor
                 *
                 * void setup() {
                 *   Stream *streams[] = {&Serial, nullptr};
                 *   module->setStreams(streams);
                 * }
                 *
                 * void loop() {
                 * }
                 * \endcode
                 */
                void setStreams(Stream **);
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_DISPLAY
                /**
                 * Return a athome::display::IDisplay interface pointer on the display currently used by the module.
                 *
                 * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                 *
                 * If there's no display interface currently used by the module, this method returns `nullptr`.
                 */
                display::IDisplay *getDisplay();
                /**
                 * Set the pointer on the athome::display::IDisplay interface used by the module.
                 *
                 * Example on Arduino to use a common cathode RGB LED and the implementation athome::module::AtHomeModule of this class:
                 *
                 * \code{.cpp}
                 * #include <AtHome.h>
                 *
                 * CommonCathodeRGBLed led(2, 3, 4); // Create a common cathode LED object and defining the pins connected to red, green and blue anodes of the LED
                 * AtHomeModule<uint16_t, 15> *module = AtHomeModule<uint16_t, 15>::getInstance(); // Create a AtHomeModule instance able to buffer 15 measures from an analog sensor
                 *
                 * void setup() {
                 *   module->setDisplay(&led);
                 * }
                 *
                 * void loop() {
                 * }
                 * \endcode
                 */
                void setDisplay(display::IDisplay *);
# endif /* DISABLE_DISPLAY */
# ifndef DISABLE_POWER_MANAGEMENT
                /**
                  * Return a athome::power::IPower interface pointer on the power supply currently used by the module.
                  *
                  * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                  *
                  * If there's no power interface currently used by the module, this method returns `nullptr`.
                  */
                power::IPower *getPowerSource();
                /**
                 * Set the pointer on the athome::power::IPower interface used by the module.
                 *
                 * For now, there is now implementation of athome::power::IPower in the library.
                 */
                void setPowerSource(power::IPower *);
# endif /* DISABLE_POWER_MANAGEMENT */
# ifndef DISABLE_SENSOR
                /**
                  * Return a athome::sensor::ISensor interface pointer on the sensor currently used by the module.
                  *
                  * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                  *
                  * If there's no sensor interface currently used by the module, this method returns `nullptr`.
                  */
                sensor::ISensor *getSensor();
                /**
                 * Set the pointer on the athome::sensor::ISensor interface used by the module.
                 *
                 * Example on Arduino using the generic AnalogSensor class to get measures from a sensor and the implementation athome::module::AtHomeModule of this class:
                 *
                 * \code{.cpp}
                 * #include <AtHome.h>
                 *
                 * AtHomeModule<uint16_t, 15> *module = AtHomeModule::<uint16_t, 15>::getInstance(); // Create a AtHomeModule instance able to buffer 15 measures from an analog sensor
                 * AnalogSensor sensor(A0); // Create an object AnalogSensor reading a sensor on pin A0 of an Arduino
                 *
                 * void setup() {
                 *   module->setSensor(&sensor);
                 * }
                 *
                 * void loop() {
                 * }
                 * \endcode
                 */
                void setSensor(sensor::ISensor *);
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_PERSISTENT_STORAGE
                /**
                  * Return a athome::storage::IStorage interface pointer on the storage currently used by the module.
                  *
                  * Warning: On some embedded targets there's no C++ runtime (so no `dynamic_cast` or `typeof`), the user will have to know and cast by himself this pointer to the correct type.
                  *
                  * If there's no storage interface currently used by the module, this method returns `nullptr`.
                  */
                storage::IStorage *getStorage();
                /**
                 * Set the pointer on the athome::storage::IStorage interface used by the module.
                 *
                 * Example on an AVR Arduino (like Uno, Pro Mini, Nano, Micro, Leonardo, Mega) using the implementation athome::module::AtHomeModule of this class and
                 * the ArduinoEEPROM class to enable storage on the internal EEPROM of these boards:
                 *
                 * \code{.cpp}
                 * #include <AtHome.h>
                 *
                 * AtHomeModule<uint16_t, 15> *module = AtHomeModule::<uint16_t, 15>::getInstance(); // Create a AtHomeModule instance able to buffer 15 measures from an analog sensor
                 * ArduinoEEPROM eeprom;
                 *
                 * void setup() {
                 *   module->setStorage(&eeprom);
                 * }
                 *
                 * void loop() {
                 * }
                 * \endcode
                 */
                void setStorage(storage::IStorage *);
# endif /* DISABLE_PERSISTENT_STORAGE */
            protected:
# ifndef DISABLE_DISPLAY
                display::IDisplay   *_display;
# endif /* DISABLE_DISPLAY */
# ifndef DISABLE_COMMUNICATION
                Stream              **_streams;
# endif /* DISABLE_COMMUNICATION */
# ifndef DISABLE_POWER_MANAGEMENT
                power::IPower       *_power;
# endif /* DISABLE_POWER_MANAGEMENT */
# ifndef DISABLE_SENSOR
                sensor::ISensor     *_sensor;
# endif /* DISABLE_SENSOR */
# ifndef DISABLE_PERSISTENT_STORAGE
                storage::IStorage   *_storage;
# endif /* DISABLE_PERSISTENT_STORAGE */
        };
    }
}

#endif /* ABASEMODULE_HPP */
