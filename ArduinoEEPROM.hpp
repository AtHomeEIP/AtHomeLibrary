#ifndef ARDUINOEEPROM_HPP
# define ARDUINOEEPROM_HPP

# if defined(__AVR__) && defined(ARDUINO)

# include "IStorage.hpp"

namespace woodBox {
    namespace storage {
        /**
         * This class is only available on AVR platforms using Arduino, as it is a wrapper of Arduino EEPROM library to implement non-volatile storage on AVR mcu.
         *
         * Usage is simple, just create an instance of this object and set is as the storage interface of your module using its setStorage method:
         *
         * \code{.cpp}
         * #include <woodBox.h>
         *
         * ArduinoEEPROM eeprom; // Create an ArduinoEEPROM instance
         * WoodBoxModule<uint16_t, 15> *module = WoodBoxModule<uint16_t, 15>::getInstance();
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

# endif

#endif /* ARDUINOEEPROM_HPP */
