#ifndef ATHOMEMODULESTREAMOUTPUTHELPER_HPP
#define ATHOMEMODULESTREAMOUTPUTHELPER_HPP
#include "AtHomeConfig.h"

#ifndef DISABLE_COMMUNICATION
#include <Stream.h>
#include "MemPrint.hpp"
#include "base64.h"
#include "AEncryptedStream.hpp"
#include "AtHomeModuleUtil.hpp"
#include "AtHomeCommunicationProtocol.hpp"
#include "Queue.hpp"

#ifndef DEFAULT_WRITER
#define DEFAULT_WRITER writeBytes
#endif  // DEFAULT_WRITER

namespace athome {
namespace module {
/**
 * Callback used by stream write helpers
 */
using StreamWriterCallback = int (*)(Stream &, const char *, size_t);

#ifndef DISABLE_ON_WRITE_CALLBACKS
/**
 * A callback type used for calling a function modifying a Stream passed as parameter
 */
using AtHomeOnWritePlugin = void (*)(Stream &);

/**
 * A plugin list to call before writing into a Stream
 */
using AtHomeBeforeOnWritePluginList = utility::Queue<AtHomeOnWritePlugin>;
/**
 * A plugin list to call after writing into a Stream
 */
using AtHomeAfterOnWritePluginList = utility::Queue<AtHomeOnWritePlugin>;
#endif /* DISABLE_ON_WRITE_CALLBACKS */

class AtHomeModuleStreamOutputHelper {
public:
  AtHomeModuleStreamOutputHelper():
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    _encryptedStreams(nullptr),
    _encryptionKey{_encryptionRawKey, sizeof(_encryptionRawKey)},
    _encryptionIV{_encryptionRawIV, sizeof(_encryptionRawIV)},
#endif /* !DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
#ifndef DISABLE_ON_WRITE_CALLBACKS
    _beforeOnWritePluginList(nullptr),
    _afterOnWritePluginList(nullptr),
#endif /* DISABLE_ON_WRITE_CALLBACKS */
    _clearStreams(nullptr)
  {
#ifndef DISABLE_ON_WRITE_CALLBACKS
    _streamOutputHelperInstance = this;
#endif /* DISABLE_ON_WRITE_CALLBACKS */
  }

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

#ifndef DISABLE_ON_WRITE_CALLBACKS
  /**
   * Add a callback to be called before writing into a stream
   */
  void setBeforeOnWriteCallback(AtHomeOnWritePlugin &plugin) {
    if (_beforeOnWritePluginList == nullptr) {
      _beforeOnWritePluginList = new AtHomeBeforeOnWritePluginList(plugin);
    } else {
      _beforeOnWritePluginList->push_back(plugin);
    }
  }

  /**
   * Add a callback to be called after writing into a stream
   */
  void setAfterOnWriteCallback(AtHomeOnWritePlugin &plugin) {
    if (_afterOnWritePluginList == nullptr) {
      _afterOnWritePluginList = new AtHomeAfterOnWritePluginList(plugin);
    } else {
      _afterOnWritePluginList->push_back(plugin);
    }
  }
#endif /* DISABLE_ON_WRITE_CALLBACKS */

  /**
   * Write bytes into the stream without any modification
   */
  static int writeBytes(Stream &stream, const char *src, size_t length) {
    size_t written = 0;
#ifndef DISABLE_ON_WRITE_CALLBACKS
    beforeOnWrite(stream);
#endif /* DISABLE_ON_WRITE_CALLBACKS */
    written = stream.write(reinterpret_cast<const uint8_t *>(src), length);
#ifndef DISABLE_ON_WRITE_CALLBACKS
    afterOnWrite(stream);
#endif /* DISABLE_ON_WRITE_CALLBACKS */
    return written;
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
    size_t written = 0;
    if (Base64encode(buffer, src, length) != encoded_length) {
      return -1;
    }
#ifndef DISABLE_ON_WRITE_CALLBACKS
    beforeOnWrite(stream);
#endif /* DISABLE_ON_WRITE_CALLBACKS */
    written = stream.write(reinterpret_cast<const uint8_t *>(buffer), encoded_length);
#ifndef DISABLE_ON_WRITE_CALLBACKS
    afterOnWrite(stream);
#endif /* DISABLE_ON_WRITE_CALLBACKS */
    return written;
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
      return writer(stream, new_line, 1);
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

#ifndef DISABLE_ON_WRITE_CALLBACKS
private:
  void _beforeOnWrite(Stream &stream) {
    AtHomeBeforeOnWritePluginList *list = _beforeOnWritePluginList;
    while (list != nullptr) {
      AtHomeOnWritePlugin *plugin = list->get();
      if (plugin != nullptr) {
        (*plugin)(stream);
      }
      list = list->next();
    }
  }

  void _afterOnWrite(Stream &stream) {
    AtHomeAfterOnWritePluginList *list = _afterOnWritePluginList;
    while (list != nullptr) {
      AtHomeOnWritePlugin *plugin = list->get();
      if (plugin != nullptr) {
        (*plugin)(stream);
      }
      list = list->next();
    }
  }
protected:
  static void beforeOnWrite(Stream &stream) {
    if (_streamOutputHelperInstance != nullptr) {
      _streamOutputHelperInstance->_beforeOnWrite(stream);
    }
  }

  static void afterOnWrite(Stream &stream) {
    if (_streamOutputHelperInstance != nullptr) {
      _streamOutputHelperInstance->_afterOnWrite(stream);
    }
  }
#else
protected:
#endif /* DISABLE_ON_WRITE_CALLBACKS */
#ifndef DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION
    arduino::AEncryptedStream **_encryptedStreams;
    arduino::AEncryptedStream::Key _encryptionKey;
    arduino::AEncryptedStream::IV _encryptionIV;
    moduleEncryptionKey _encryptionRawKey;
    moduleEncryptionIV _encryptionRawIV;
#endif /* !DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION */
#ifndef DISABLE_ON_WRITE_CALLBACKS
    AtHomeBeforeOnWritePluginList *_beforeOnWritePluginList;
    AtHomeAfterOnWritePluginList *_afterOnWritePluginList;
#endif /* DISABLE_ON_WRITE_CALLBACKS */
    Stream **_clearStreams;
#ifndef DISABLE_ON_WRITE_CALLBACKS
    static AtHomeModuleStreamOutputHelper *_streamOutputHelperInstance;
#endif /* DISABLE_ON_WRITE_CALLBACKS */
};
} // module
} // athome

#endif  // !DISABLE_COMMUNICATION

#endif  // ATHOMEMODULESTREAMOUTPUTHELPER_HPP
