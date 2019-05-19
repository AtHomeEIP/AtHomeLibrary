#ifndef ATHOMEMODULESTREAMOUTPUTHELPER_HPP
#define ATHOMEMODULESTREAMOUTPUTHELPER_HPP
#include "AtHomeConfig.h"

#ifndef DISABLE_COMMUNICATION
#include <Stream.h>
#include "MemPrint.hpp"
#include "base64.h"
#include "AEncryptedStream.hpp"
#include "AtHomeModuleUtil.hpp"

#ifndef DEFAULT_WRITER
#define DEFAULT_WRITER writeBytes
#endif  // DEFAULT_WRITER

namespace athome {
namespace module {
/**
 * Callback used by stream write helpers
 */
using StreamWriterCallback = int (*)(Stream &, const char *, size_t);

class AtHomeModuleStreamOutputHelper {
public:
  AtHomeModuleStreamOutputHelper():
#if !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION)
    _encryptedStreams(nullptr),
    _encryptionKey{_encryptionRawKey, sizeof(_encryptionRawKey)},
    _encryptionIV{_encryptionRawIV, sizeof(_encryptionRawIV)},
#endif /* !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */
    _clearStreams(nullptr)
  {}

  void setClearStreams(Stream **streams) {
      _clearStreams = streams;
  }

  Stream **getClearStreams() {
      return _clearStreams;
  }

#if !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION)
  /**
   * `moduleEncryptionKey` type represents a key for a cipher protecting an
   * unsecure stream
   */
  typedef uint8_t moduleEncryptionKey[32];
  /**
   * `moduleEncryptionIV` type represents an IV used for a cipher protecting an
   * unsecure stream
   */
  typedef uint8_t moduleEncryptionIV[12];
#endif /* !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */
  /**
   * Write bytes into the stream without any modification
   */
  static int writeBytes(Stream &stream, const char *src, size_t length) {
    return stream.write(reinterpret_cast<const uint8_t *>(src), length);
  }

  /**
   * Write bytes into the stream without any modification
   */
  template <typename U>
  static int writeBytes(Stream &stream, const U &data) {
    return writeBytes(stream, reinterpret_cast<const char *>(&data), sizeof(U));
  }

  /**
   * Write bytes into the stream enabling to choose an encoder used to write them
   */
  static int writeBytesHelper(Stream &stream, const char *src, size_t length, StreamWriterCallback writer = DEFAULT_WRITER) {
    return writer(stream, src, length);
  }

  /**
   * Write bytes into the stream enabling to choose an encoder used to write them
   */
  template <typename U>
  static int writeBytesHelper(Stream &stream, const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
    return writer(stream, reinterpret_cast<const char *>(&data), sizeof(U));
  }

  /**
   * Convert data to Base64 and write them into the stream.
   */
  static int writeBase64EncodedBytes(Stream &stream, const char *src, size_t length) {
    size_t encoded_length = base64_encoded_length(length);
    char buffer[encoded_length];
    if (Base64encode(buffer, src, length) != encoded_length) {
      return -1;
    }
    return stream.write(reinterpret_cast<const uint8_t *>(buffer), encoded_length);
  }

  /**
   * Convert data to Base64 and write them into the stream.
   */
  template <typename U>
  static int writeBase64EncodedBytes(Stream &stream, const U &data) {
    return writeBase64EncodedBytes(stream, reinterpret_cast<const char *>(&data), sizeof(U));
  }

  /**
   * Print data into a stream using a customisable write function.
   */
  template <typename U, size_t reserved = 64>
  static int printHelper(Stream &stream, const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
      char buffer[reserved];
      arduino::MemPrint memPrinter(buffer);
      size_t len = memPrinter.print(data);
      return writer(stream, buffer, len);
  }

  /**
   * Print data into a stream using a customisable write function and append a line feed to it.
   */
  template <typename U, size_t reserved = 64>
  static int printHelperLn(Stream &stream, const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
      printHelper<U, reserved>(stream, data, writer);
      char new_line[2] = { ATHOME_NEW_LINE, '\0' };
      writer(stream, new_line, 1);
  }

  /**
   * Broadcast the data passed as parameter over all module streams.
   */
  template <typename U, size_t reserved = 64>
  void broadcast(const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
    char buffer[reserved];
    arduino::MemPrint memPrinter(buffer);
    size_t len = memPrinter.print(data);
    if (_clearStreams != nullptr) {
      for (size_t i = 0; _clearStreams[i] != nullptr; i++) {
        writer(*_clearStreams[i], buffer, len);
      }
    }
  #ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
  #warning "Not implemented"
  #endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
  }

  /**
   * Broadcast the data passed as parameter over all module streams, followed by
   * line return string "\r\n".
   */
  template <typename U>
  void broadcastln(const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
    broadcast(data, writer);
    broadcast(ATHOME_NEW_LINE, writer);
  }

  void raw_broadcast(const char *src, size_t length, StreamWriterCallback writer = DEFAULT_WRITER) {
    if (_clearStreams != nullptr) {
      for (size_t i = 0; _clearStreams[i] != nullptr; i++) {
        writer(*_clearStreams[i], src, length);
      }
    }
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    if (_encryptedStreams != nullptr) {
      for (size_t i = 0; _encryptedStreams[i] != nullptr; i++) {
        writer(*_encryptedStreams[i], src, length);
      }
    }
#endif /* DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
  }

  template <typename U>
  void raw_broadcast(const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
    raw_broadcast(reinterpret_cast<const char *>(&data), sizeof(U), writer);
  }

  /**
   * Broadcast empty string
   */
  void broadcast_end_of_string(StreamWriterCallback writer = DEFAULT_WRITER) { broadcast('\0', writer); }

  /**
   * Serialize an object as String and broadcast it
   */
  template <typename U>
  void broadcast_raw_string(const U &data, StreamWriterCallback writer = DEFAULT_WRITER) {
    broadcast(data, writer);
    broadcast_end_of_string(writer);
  }

  /**
   * Serialize an integer as a variable size integer using Google spec
   */
  template <typename U>
  void broadcast_varuint(U data, StreamWriterCallback writer = DEFAULT_WRITER) {
    if (is_zero<U>(data)) {
      raw_broadcast<uint8_t>(0, writer);
      return;
    }
    while (is_non_zero<U>(data)) {
      uint8_t buffer = 0;
      buffer |= (data & 0b01111111);
      data >>= 7;
      if (is_non_zero<U>(data)) {
        buffer |= 0b10000000;
      }
      raw_broadcast<uint8_t>(buffer, writer);
    }
  }

  #ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
  /**
   * Set the array of unsecure streams used to communicate, terminated by
   * nullptr
   */
  void setEncryptedStreams(arduino::AEncryptedStream **streams) {
    _encryptedStreams = streams;
    if (_encryptedStreams != nullptr) {
      for (size_t i = 0; _encryptedStreams[i] != nullptr; i++) {
        _encryptedStreams[i]->setKey(_encryptionKey);
        _encryptedStreams[i]->setIV(_encryptionIV);
      }
    }
  }

  /**
   * Return the array of unsecure streams used to communicate, terminated by
   * nullptr
   */
  arduino::AEncryptedStream **getEncryptedStreams() {
    return _encryptedStreams;
  }

  /**
   * Set the encryption key used to protect unsecure streams
   */
  void setEncryptionKey(const moduleEncryptionKey &key) {
    memcpy(_encryptionRawKey, key, sizeof(_encryptionRawKey));
#ifndef DISABLE_PERSISTENT_STORAGE
    onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
  }

  const moduleEncryptionKey &getEncryptionKey() const {
    return _encryptionRawKey;
  }

  /**
   * Set the initialization vector used by a cipher to protect unsecure streams
   */
  void setEncryptionIV(const moduleEncryptionIV &iv) {
    memcpy(_encryptionRawIV, iv, sizeof(_encryptionRawIV));
#ifndef DISABLE_PERSISTENT_STORAGE
    onBackupOnStorage();
#endif /* DISABLE_PERSISTENT_STORAGE */
  }

  const moduleEncryptionIV &getEncryptionIV() const { return _encryptionRawIV; }
#endif /* !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */

protected:
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    arduino::AEncryptedStream **_encryptedStreams;
    arduino::AEncryptedStream::Key _encryptionKey;
    arduino::AEncryptedStream::IV _encryptionIV;
    moduleEncryptionKey _encryptionRawKey;
    moduleEncryptionIV _encryptionRawIV;
#endif /* !DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
    Stream **_clearStreams;
};
} // module
} // athome

#endif  // !DISABLE_COMMUNICATION

#endif  // ATHOMEMODULESTREAMOUTPUTHELPER_HPP
