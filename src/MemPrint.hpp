#ifndef MEMPRINT_HPP
#define MEMPRINT_HPP
#include <Print.h>
#include <stddef.h>
#include <stdint.h>
#include "base64.h"

namespace athome {
namespace arduino {
class MemPrint : public Print {
 public:
  MemPrint(char *buffer):len(0),buffer(reinterpret_cast<uint8_t *>(buffer)),Print() {}
  MemPrint(const MemPrint &other):len(other.len),buffer(other.buffer),Print() {}
  MemPrint &operator=(const MemPrint &other) {
    if (this != &other) {
      len = other.len;
      buffer = other.buffer;
    }
    return *this;
  }
  ~MemPrint() {}

  size_t write(uint8_t b) {
    buffer[len++] = b;
    return 1;
  }

  void setBuffer(char *buffer) {
    this->buffer = reinterpret_cast<uint8_t *>(buffer);
    len = 0;
  }

 private:
  size_t    len;
  uint8_t * buffer;
};
}
}

#endif /* MEMPRINT_HPP */
