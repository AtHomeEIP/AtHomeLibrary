#ifndef DISABLE_CRC
#include <stdint.h>
/**
 * Implementation of the AVR crc16_update function from
 * https://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
 */
uint16_t portable_crc16_update(uint16_t crc, uint8_t a) {
  crc ^= a;
  for (uint8_t i = 0; i < 8; ++i) {
    if (crc & 1) {
      crc = (crc >> 1) ^ 0xA001;
    } else {
      crc = (crc >> 1);
    }
  }
  return crc;
}
#endif  // DISABLE_CRC
