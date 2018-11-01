#ifndef CRC_HPP
#define CRC_HPP
#ifdef __AVR__
#include <util/crc16.h>
#else
#include <stdint.h>
extern "C" {
uint16_t crc16_update(uint16_t, uint8_t);
}
#endif  // __AVR__
#include <stddef.h>
namespace athome {
namespace util {
namespace checksum {
constexpr uint16_t rec_crc16_update(uint16_t crc, uint8_t i) {
  return (i < 8) ? rec_crc16_update(
                       (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1), i + 1)
                 : crc;
}
constexpr uint16_t const_crc16_update(uint16_t crc, uint8_t a) {
  return rec_crc16_update(crc ^ a, 1);
}
template <typename T>
constexpr uint16_t rec_crc16(const uint8_t *data, uint16_t crc, size_t i) {
  return (i >= sizeof(T))
             ? crc
             : rec_crc16<T>(data, const_crc16_update(crc, data[i]), i++);
}
template <typename T>
constexpr uint16_t crc16(const T &data) {
  return rec_crc16<T>(reinterpret_cast<const uint8_t *>(&data), 0U, 0U);
}
}  // namespace checksum
}  // namespace util
}  // namespace athome
#endif  // CRC_HPP
