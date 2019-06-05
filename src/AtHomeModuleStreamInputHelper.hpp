#ifndef ATHOMEMODULESTREAMINPUTHELPER_HPP
#define ATHOMEMODULESTREAMINPUTHELPER_HPP
#include "AtHomeConfig.h"

#ifndef DISABLE_COMMUNICATION
#include <Stream.h>
#include "CRC.hpp"
#include "AtHomeModuleUtil.hpp"
#include "base64.h"

namespace athome {
namespace module {

/**
 * Callback used by stream read helpers
 */
using StreamReaderCallback = int (*)(Stream &, char *, size_t);

class AtHomeModuleStreamInputHelper {
public:
  /**
   * Generic function allowing to read from a Stream up until n bytes
   * into memory
   */
  static int readBytes(Stream &stream, char *dest, size_t u) {
    int i = 0;
    int j = 0;
    unsigned long timeout = millis() + DEFAULT_STREAM_TIMEOUT;
    do {
      j = stream.readBytes(dest, u - i);
      if (j < 0) {
        return i;
      }
      i += j;
    } while (i < u && millis() < timeout);
    return i;
  }

  /**
   * Read the content of an object from a Stream
   */
  template <typename U>
  static int readBytes(Stream &stream, U &dest) {
    return readBytes(stream, reinterpret_cast<char *>(&dest), sizeof(U));
  }

  template <typename U>
  static int readBytesHelper(Stream &stream, U &dest, StreamReaderCallback reader = readBytes) {
    return reader(stream, reinterpret_cast<char *>(&dest), sizeof(U));
  }

  /**
   * Read base64 encoded stream input and copy the decoded output
   * into the destination
   */
  static int readBase64EncodedBytes(Stream &stream, char *dest, size_t count) {
    int encoded_length = base64_encoded_length(count);
    char buffer[encoded_length];
    int read_length = readBytes(stream, buffer, encoded_length);

    if (read_length < encoded_length) {
      return -1;
    }
    return Base64decode(reinterpret_cast<char *>(&dest), buffer);
  }

  /**
   * Read base64 encoded stream input and copy the decoded output
   * into the destination
   */
  template <typename U>
  static int readBase64EncodedBytes(Stream &stream, U &dest) {
    return readBase64EncodedBytes(stream, reinterpret_cast<char *>(&dest), sizeof(U));
  }

  /**
   * Read an object content from a Stream and perform CRC verification
   * if available
   */
  template <typename U>
  int securedReadBytes(Stream &stream, U &dest, StreamReaderCallback reader = readBytes) {
    uint16_t crcRef;
    if (reader(stream, reinterpret_cast<char *>(&crcRef), sizeof(crcRef)) < 0) {
      return -1;
    }
    int size = reader(stream, reinterpret_cast<char *>(&dest), sizeof(U));
#ifndef DISABLE_CRC
    uint16_t crc = utility::checksum::crc16_it<U>(dest);
    if (crc != crcRef) {
      return CRC_ERROR;
    }
#endif  // DISABLE_CRC
    return size;
  }

  template <typename U, char del = '\0', uint8_t init = 0>
  int securedReadBytesUntil(Stream &stream, U &dest) {
    uint16_t crcRef;
    if (readBytes<uint16_t>(stream, crcRef) < 0) {
      return -1;
    }
    char *ptr = reinterpret_cast<char *>(&dest);
    memset(ptr, init, sizeof(U));
    int size = stream.readBytesUntil(del, ptr, sizeof(U) - 1);
    ptr[size] = del;
    if (size < 0) {
      return -1;
    }
#ifndef DISABLE_CRC
    uint16_t crc = utility::checksum::crc16_it<U>(dest, size);
    if (crc != crcRef) {
      return CRC_ERROR;
    }
#endif  // DISABLE_CRC
    return size;
  }
};
}
}
#endif  // !DISABLE_COMMUNICATION

#endif  // ATHOMEMODULESTREAMINPUTHELPER_HPP
