#ifndef ARDUINOEEPROM_HPP
# define ARDUINOEEPROM_HPP

# include "AtHomeConfig.h"
# if defined(__AVR__) && !defined(DISABLE_PERSISTENT_STORAGE) && !defined(DISABLE_AVR_EEPROM)

# include "IStorage.hpp"

namespace athome {
    namespace storage {
        /**
         * This class is only available on AVR platforms using Arduino, as it is a wrapper of Arduino EEPROM library to implement non-volatile storage on AVR mcu.
         *
         * Usage is simple, just create an instance of this object and set is as the storage interface of your module using its setStorage method:
         *
         * \code{.cpp}
         * #include <AtHome.h>
         *
         * ArduinoEEPROM eeprom; // Create an ArduinoEEPROM instance
         * AtHomeModule<uint16_t, 15> *module = AtHomeModule<uint16_t, 15>::getInstance();
         *
         * void setup() {
         *   module->setStorage(&eeprom);
         * }
         *
         * void loop() {
         * }
         * \endcode
         */
        class ArduinoEEPROM : public IStorage {
        public:
            ArduinoEEPROM();
            ArduinoEEPROM(const ArduinoEEPROM &) = delete;
            ArduinoEEPROM &operator=(const ArduinoEEPROM &) = delete;
            ~ArduinoEEPROM();

            void read(size_t, void *, size_t);
            void write(size_t, const void *, size_t);
        };
    }
}

# endif /* defined(__AVR__) && !defined(DISABLE_PERSISTENT_STORAGE) && !defined(DISABLE_AVR_EEPROM) */

#endif /* ARDUINOEEPROM_HPP */
