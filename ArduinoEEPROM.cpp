#include "AtHomeConfig.h"
#if defined(__AVR__) && !defined(DISABLE_PERSISTENT_STORAGE) && !defined(DISABLE_AVR_EEPROM)

# include "ArduinoEEPROM.hpp"
# include <EEPROM.h>

namespace athome {
    namespace storage {
        ArduinoEEPROM::ArduinoEEPROM() {}

        ArduinoEEPROM::~ArduinoEEPROM() {}

        void ArduinoEEPROM::read(size_t offset, void *dest, size_t len) {
            uint8_t *ptr = reinterpret_cast<uint8_t *>(dest);
            size_t end = offset + len;
            for (size_t i = offset; i < end; i++) {
                *ptr = EEPROM.read(i);
                ptr++;
            }
        }

        void ArduinoEEPROM::write(size_t offset, const void *src, size_t len) {
            const uint8_t *ptr = reinterpret_cast<const uint8_t *>(src);
            size_t end = offset + len;
            for (size_t i = offset; i < end; i++) {
                EEPROM.update(i, *ptr);
                ptr++;
            }
        }
    }
}

#endif /* defined(__AVR__) && !defined(DISABLE_PERSISTENT_STORAGE) && !defined(DISABLE_AVR_EEPROM) */
