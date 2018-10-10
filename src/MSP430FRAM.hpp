#ifndef MSP430FRAM_HPP
#define MSP430FRAM_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_PERSISTENT_STORAGE) && !defined(DISABLE_MSP430_FRAM)
#ifdef __MSP430__
#include <driverlib.h>
#include <stddef.h>
#include <stdint.h>
#include <IStorage.hpp>
#ifndef ATHOME_FRAM_STORAGE_SIZE
#define ATHOME_FRAM_STORAGE_SIZE 255
#endif

namespace athome {
namespace storage {
#if defined(__TI_COMPILER_VERSION__)
#pragma PERSISTENT(FRAM_storage)
extern uint8_t FRAM_storage[ATHOME_FRAM_STORAGE_SIZE];
#elif defined(__IAR_SYSTEMS_ICC__)
extern __persistent uint8_t FRAM_storage[ATHOME_FRAM_STORAGE_SIZE];
#else
extern uint8_t __attribute__((persistent))
FRAM_storage[ATHOME_FRAM_STORAGE_SIZE];
#endif

/**
 * Singleton enabling to store persistent data in the FRAM of TI Launchpads.
 *
 * The size of the persistent storage is defined by the macro
 * ATHOME_FRAM_STORAGE_SIZE and has a default value of 255 bytes.
 *
 * This can be overriden by the user by defining this macro at the compilation,
 * such as passing the `-DATHOME_FRAM_STORAGE_SIZE=` option with GCC
 */
class MSP430FRAM : public IStorage {
 public:
  MSP430FRAM(const MSP430FRAM &) = delete;
  MSP430FRAM &operator=(const MSP430FRAM &) = delete;

  static MSP430FRAM &getInstance() {
    static MSP430FRAM instance;
    return _instance;
  }
  virtual void read(size_t, void *, size_t);
  virtual void write(size_t, const void *, size_t);

 private:
  MSP430FRAM();
  ~MSP430FRAM();
};
}  // namespace storage
}  // namespace athome

#endif /* __MSP430__ */
#endif /* !defined(DISABLE_PERSISTENT_STORAGE) && \
          !defined(DISABLE_MSP430_FRAM) */
#endif /* MSP430FRAM_HPP */
