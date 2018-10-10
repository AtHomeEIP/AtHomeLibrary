#ifndef AENCRYPTEDSTREAM_HPP
#define AENCRYPTEDSTREAM_HPP
#include <Stream.h>
#include <stdint.h>
#include "Pair.hpp"

namespace athome {
namespace arduino {
/**
 * Abstract class used to protect an Arduino Stream with encryption
 */
class AEncryptedStream : public Stream {
 protected:
  /**
   * Header of a block of an encrypted stream containing the following fields:
   *
   * - length: number of bytes in the block (doesn't need any padding)
   * - reserved: reserved bits for later fields to add to the struct
   */
  struct BlockHeader {
    uint8_t length : 4;
    uint8_t reserved : 4;
  };
  /**
   * Structure representing an encrypted block:
   *
   * - header: a header structure describing the block
   * - data: the data bytes contained in the block, up to 15 bytes
   */
  struct Block {
    BlockHeader header;
    uint8_t data[15];
  };

 public:
  /**
   * Pair storing the key used by the cipher:
   *
   * - first: a reference to the memory where the key is stored
   * - second: the size in bytes of the key
   */
  using Key = utility::container::Pair<const uint8_t *, size_t>;
  /**
   * Pair storing the IV used by the cipher:
   *
   * - first: a reference to the memory where the iv is stored
   * - second: the size in bytes of the iv
   */
  using IV = utility::container::Pair<const uint8_t *, size_t>;

 protected:
  /**
   * Pair used to initialize a cipher:
   * - fist: a reference to a key
   * - second: a pointer to an optional IV (nullptr if not present)
   */
  using CipherMaterials = utility::container::Pair<const Key &, const IV *>;

 public:
  /**
   * Constructor of the encrypted stream, mandatory taking a reference to a
   * stream to protect
   */
  explicit AEncryptedStream(Stream &);
  AEncryptedStream(const AEncryptedStream &) = delete;
  AEncryptedStream &operator=(const AEncryptedStream &) = delete;

  /**
   * Return a reference on the base protected Arduino Stream by this class
   */
  Stream &getBaseStream() { return _base; }
  /**
   * Return a reference on the base protected Arduino Stream by this class
   */
  const Stream &getBaseStream() const { return _base; }
  /**
   * Set a reference on the base protected Arduino Stream by this class
   */
  void setBaseStream(Stream &);
  /**
   * Return a pointer on the key set in the cipher if set, else return nullptr
   */
  const Key *getKey() const { return _key; }
  /**
   * Set the key used by the cipher by passing a reference on it
   */
  void setKey(const Key &);
  /**
   * Return a pointer on the IV used by the cipher if set, else return nullptr
   */
  const IV *getIV() const { return _iv; }
  /**
   * Set the IV used by the cipher by passing a reference on it
   */
  void setIV(const IV &);

 public:
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *b, size_t len) {
    size_t n = 0;
    while (len--) {
      if (!write(*b++)) {
        break;
      }
      n++;
    }
    return n;
  }
  virtual int read();
  virtual int peek();
  virtual int available();
  virtual void flush();

 protected:
  /**
   * Set the key and IV in the implementation of the cipher
   */
  virtual bool initializeCipher(const CipherMaterials &) = 0;
  /**
   * Encrypt the block passed by reference
   */
  virtual void encrypt(Block &) = 0;
  /**
   * Decrypt the block passed by reference
   */
  virtual void decrypt(Block &) = 0;

 private:
  void reset();
  void updateReadBlock();

 private:
  Stream &_base;
  const Key *_key;
  const IV *_iv;
  size_t _readBlockPos;
  bool _initialized;
  bool _readBlockDecrypted;
  Block _writeBlock;
  Block _readBlock;
};
}  // namespace arduino
}  // namespace athome

#endif /* AENCRYPTEDSTREAM_HPP */
