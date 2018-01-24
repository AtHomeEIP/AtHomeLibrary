#ifndef ARDUINOEEPROM_HPP
# define ARDUINOEEPROM_HPP

# if defined(__AVR__) && defined(ARDUINO)

# include "IStorage.hpp"

namespace woodBox {
    namespace storage {
        class ArduinoEEPROM {
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
