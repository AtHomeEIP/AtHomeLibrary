#include "AtHomeConfig.h"
#if !defined(DISABLE_COMMUNICATION) && \
    !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION)
#include "ChaChaEncryptedStream.hpp"

namespace athome {
namespace arduino {
ChaChaEncryptedStream::ChaChaEncryptedStream(Stream &stream)
    : _counter({0, 1, 2, 3, 4, 5, 6, 7}), AEncryptedStream(stream) {}

bool ChaChaEncryptedStream::initializeCipher(
    const AEncryptedStream::CipherMaterials &materials) {
  const Key &key = materials.first;
  const IV *iv = (materials.second != nullptr) ? materials.second : getIV();

  return !(iv == nullptr || !_cipher.setKey(&key.first, key.second) ||
           !_cipher.setIV(&iv->first, iv->second) ||
           !_cipher.setCounter(_counter, sizeof(_counter)));
}
}  // namespace arduino
}  // namespace athome

#endif /* !defined(DISABLE_COMMUNICATION) && \
          !defined(DISABLE_UNSECURE_COMMUNICATION_ENCRYPTION) */
