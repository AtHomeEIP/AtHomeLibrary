#ifndef CHACHAENCRYPTEDSTREAM_HPP
#define CHACHAENCRYPTEDSTREAM_HPP
#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && \
    !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION)
#include <Chacha.h>
#include <Crypto.h>
#include <stdint.h>
#include "AEncryptedStream.hpp"

namespace athome {
namespace arduino {
class ChaChaEncryptedStream : public AEncryptedStream {
 public:
  typedef uint8_t counter[8];

  explicit ChaChaEncryptedStream(Stream &);
  ChaChaEncryptedStream(const ChaChaEncryptedStream &) = delete;
  ChaChaEncryptedStream &operator=(const ChaChaEncryptedStream &) = delete;
  virtual ~ChaChaEncryptedStream() {}

  const counter &getCounter() const { return _counter; }
  void setCounter(const counter &cnt) {
    memcpy(_counter, cnt, sizeof(_counter));
  }

 protected:
  virtual bool initializeCipher(const CipherMaterials &);
  virtual void encrypt(Block &block) {
    _cipher.encrypt(reinterpret_cast<uint8_t *>(&block),
                    reinterpret_cast<const uint8_t *>(&block), sizeof(block));
  }
  virtual void decrypt(Block &block) {
    _cipher.encrypt(reinterpret_cast<uint8_t *>(&block),
                    reinterpret_cast<const uint8_t *>(&block), sizeof(block));
  }

 private:
  counter _counter;
  ChaCha _cipher;
};
}  // namespace arduino
}  // namespace athome
#endif /* !defined(DISABLE_COMMUNICATION) && \
          !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */
#endif /* CHACHAENCRYPTEDSTREAM_HPP */
