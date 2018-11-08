#ifndef CRC_HPP
#define CRC_HPP
#ifndef DISABLE_CRC
#define CRC_ERROR -2
#include <stddef.h>
#include <stdint.h>
uint16_t portable_crc16_update(uint16_t, uint8_t);
namespace athome {
namespace utility {
namespace checksum {
using CRC16 = uint16_t;
constexpr CRC16 rec_crc16_update(CRC16 crc, uint8_t i) {
  return (i < 8) ? rec_crc16_update(
                       (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1), i + 1)
                 : crc;
}
constexpr CRC16 const_crc16_update(CRC16 crc, uint8_t a) {
  return rec_crc16_update(crc ^ a, 1);
}
template <typename T>
constexpr CRC16 rec_crc16(const uint8_t *data, CRC16 crc, size_t i) {
  return (i >= sizeof(T))
             ? crc
             : rec_crc16<T>(data, const_crc16_update(crc, data[i]), i++);
}
template <typename T>
constexpr CRC16 crc16(const T &data) {
  return rec_crc16<T>(reinterpret_cast<const uint8_t *>(&data), 0U, 0U);
}

template <typename T>
CRC16 crc16_it(const T &data) {
  CRC16 crc = 0;
  const uint8_t *mem = reinterpret_cast<const uint8_t *>(&data);
  for (size_t i = 0; i < sizeof(T); i++) {
    crc = portable_crc16_update(crc, mem[i]);
  }
  return crc;
}
}  // namespace checksum
}  // namespace utility
}  // namespace athome
#endif  // DISABLE_CRC
#endif  // CRC_HPP
