#include "AtHomeConfig.h"
#if !defined(DISABLE_PERSISTENT_STORAGE) && !defined(DISABLE_MSP430_FRAM)
#ifdef __MSP430__
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "MSP430FRAM.hpp"

namespace athome {
namespace storage {
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(FRAM_storage)
uint8_t FRAM_storage[ATHOME_FRAM_STORAGE_SIZE] = {1};
#elif defined(__IAR_SYSTEMS_ICC__)
__persistent uint8_t FRAM_storage[ATHOME_FRAM_STORAGE_SIZE] = {1};
#else
uint8_t __attribute__((persistent))
FRAM_storage[ATHOME_FRAM_STORAGE_SIZE] = {1};
#endif

MSP430FRAM::MSP430FRAM() {}

MSP430FRAM::~MSP430FRAM() {}

void MSP430FRAM::read(size_t offset, void *dest, size_t size) {
  if (offset + size < sizeof(FRAM_storage)) {
    memcpy(dest, FRAM_storage + offset, size);
  }
}

void MSP430FRAM::write(size_t offset, const void *src, size_t size) {
  if (offset + size < sizeof(FRAM_storage)) {
    memcpy(FRAM_storage + offset, src, size);
  }
}

MSP430FRAM *MSP430FRAM::_instance = nullptr;
}  // namespace storage
}  // namespace athome

#endif /* __MSP430__ && __MSP430_HAS_FRAM__ */
#endif /* !defined(DISABLE_PERSISTENT_STORAGE) && \
          !defined(DISABLE_MSP430_FRAM) */