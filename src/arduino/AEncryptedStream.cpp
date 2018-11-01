#include "AEncryptedStream.hpp"
#include <string.h>

namespace athome {
namespace arduino {
AEncryptedStream::AEncryptedStream(Stream &base)
    : _base(base),
      _key(nullptr),
      _iv(nullptr),
      _readBlockPos(0),
      _initialized(false),
      _readBlockDecrypted(false),
      Stream() {
  memset(&_writeBlock, 0, sizeof(_writeBlock));
}

void AEncryptedStream::setBaseStream(Stream &base) {
  reset();
  _base = base;
}

void AEncryptedStream::setKey(const Key &key) {
  _key = &key;
  CipherMaterials materials(key, _iv);
  if (initializeCipher(materials)) {
    _initialized = true;
  }
}

void AEncryptedStream::setIV(const IV &iv) {
  _iv = &iv;
  if (_key != nullptr) {
    CipherMaterials materials(*_key, _iv);
    if (initializeCipher(materials)) {
      _initialized = true;
    }
  }
}

size_t AEncryptedStream::write(uint8_t b) {
  if (!_initialized) {
    return 0;
  }
  _writeBlock.data[_writeBlock.header.length++] = b;
  if (_writeBlock.header.length == sizeof(_writeBlock.data)) {
    encrypt(_writeBlock);
    _base.write(reinterpret_cast<const uint8_t *>(&_writeBlock),
                sizeof(_writeBlock));
    memset(&_writeBlock, 0, sizeof(_writeBlock));
  }
  return 1;
}

int AEncryptedStream::read() {
  if (!_initialized) {
    return -1;
  }
  updateReadBlock();
  if (_readBlockDecrypted) {
    int b = _readBlock.data[_readBlockPos++];
    if (_readBlockPos >= _readBlock.header.length) {
      _readBlockDecrypted = false;
      _readBlockPos = 0;
    }
    return b;
  }
}

int AEncryptedStream::peek() {
  if (!_initialized) {
    return -1;
  }
  updateReadBlock();
  if (_readBlockDecrypted) {
    return _readBlock.data[_readBlockPos];
  }
}

int AEncryptedStream::available() {
  return (!_initialized || !_readBlockDecrypted)
             ? 0
             : _readBlock.header.length - _readBlockPos;
}

void AEncryptedStream::flush() {
  if (_initialized) {
    encrypt(_writeBlock);
    _base.write(reinterpret_cast<const uint8_t *>(&_writeBlock),
                sizeof(_writeBlock));
  }
  memset(&_writeBlock, 0, sizeof(_writeBlock));
  _readBlockDecrypted = false;
  _readBlockPos = 0;
}

void AEncryptedStream::updateReadBlock() {
  if (!_readBlockDecrypted) {
    size_t len =
        _base.readBytes(reinterpret_cast<char *>(&_readBlock) + _readBlockPos,
                        sizeof(_readBlock) - _readBlockPos);
    if (len >= sizeof(_readBlock)) {
      decrypt(_readBlock);
      _readBlockPos = 0;
      _readBlockDecrypted = true;
    } else {
      _readBlockPos += len;
    }
  }
}

void AEncryptedStream::reset() {
  _key = nullptr;
  _iv = nullptr;
  _readBlockPos = 0;
  _initialized = false;
  _readBlockDecrypted = false;
  memset(&_writeBlock, 0, sizeof(_writeBlock));
}
}  // namespace arduino
}  // namespace athome
