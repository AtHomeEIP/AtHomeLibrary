#ifndef ATHOMEMODULESTREAMIO_HPP
#define ATHOMEMODULESTREAMIO_HPP
#include "AtHomeConfig.h"

#ifndef DISABLE_COMMUNICATION

#include "AtHomeModuleStreamInputHelper.hpp"
#include "AtHomeModuleStreamOutputHelper.hpp"

namespace athome {
namespace module {
class AtHomeModuleStreamIO : public AtHomeModuleStreamInputHelper, public AtHomeModuleStreamOutputHelper {
public:
  AtHomeModuleStreamIO():AtHomeModuleStreamInputHelper(),AtHomeModuleStreamOutputHelper() {}

  #ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
  /**
   * Flush encrypted Stream used for broadcast
   */
  void flush_encryptedStreams() {
    if (_encryptedStreams == nullptr) {
      return;
    }
    for (size_t i = 0; _encryptedStreams[i] != nullptr; i++) {
      _encryptedStreams[i]->flush();
    }
  }
#endif  // DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION

  /**
   * Flush all broadcast Stream
   */
  void broadcast_flush() {
    flushStreams();
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    flush_encryptedStreams();
#endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
  }

  /**
   * Flush all streams used by the module.
   */
  void flushStreams() {
    if (_clearStreams == nullptr) {
      return;
    }
    for (size_t i = 0; _clearStreams[i] != nullptr; i++) {
      _clearStreams[i]->flush();
    }
  }
};
} // module
} // athome

#endif  // !DISABLE_OMMUNICATION

#endif  // ATHOMEMODULESTREAMIO_HPP
