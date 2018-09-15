#ifndef AENCRYPTEDSTREAM_HPP
#define AENCRYPTEDSTREAM_HPP
#include <Stream.h>
#include <stdint.h>

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
   * - application: a boolean flag indicating if the data is intended for the
   * application or stream level
   * - length: number of bytes in the block (doesn't need any padding)
   * - reserved: reserved bits for later fields to add to the struct
   */
  struct BlockHeader {
    uint8_t application : 1;
    uint8_t length : 4;
    uint8_t reserved : 3;
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
   * Structure storing the key used by the cipher:
   *
   * - key: a reference to the memory where the key is stored
   * - keyLength: the size in bytes of the key
   */
  struct Key {
    const uint8_t &key;
    size_t keyLength;
  };
  /**
   * Structure storing the IV used by the cipher:
   *
   * - iv: a reference to the memory where the iv is stored
   * - ivLength: the size in bytes of the iv
   */
  struct IV {
    const uint8_t &iv;
    size_t ivLength;
  };

 public:
  /**
   * Constructor of the encrypted stream, mandatory taking a reference to a
   * stream to protect
   */
  EncryptedStream(Stream &);
  EncryptedStream(const EncryptedStream &) = delete;
  EncryptedStream &operator=(const EncryptedStream &) = delete;

  /**
   * Return a reference on the base protected Arduino Stream by this class
   */
  Stream &getProtectedStream();
  /**
   * Return a reference on the base protected Arduino Stream by this class
   */
  const Stream &getProtectedStream() const;
  /**
   * Set a reference on the base protected Arduino Stream by this class
   */
  void setProtectedStream(Stream &);
  /**
   * Return a pointer on the key set in the cipher if set, else return nullptr
   */
  const Key *getKey() const;
  /**
   * Set the key used by the cipher by passing a reference on it
   */
  void setKey(const Key &);
  /**
   * Return a pointer on the IV used by the cipher if set, else return nullptr
   */
  const IV *getIV() const;
  /**
   * Set the IV used by the cipher by passing a reference on it
   */
  void setIV(const IV &);

 public:
  virtual size_t write(uint8_t);
  virtual int read();
  virtual int peek();
  virtual int available();
  virtual void flush();

 protected:
  /**
   * Set the key in the implementation of the cipher
   */
  virtual void setCipherKey(const Key &) = 0;
  /**
   * Set the IV in the implementation of the cipher
   */
  virtual void setCipherIV(const IV &) = 0;
  /**
   * Encrypt the block passed by reference
   */
  virtual void encrypt(Block &) = 0;
  /**
   * Decrypt the block passed by reference
   */
  virtual void decrypt(Block &) = 0;

 private:
  Stream &_base;
  Key *_key;
  IV *_iv;
  Block _decryptedBlock;
  Block _incomingBlock;
  size_t _decryptedBlockPos;
  size_t _incomingBlockPos;
};
}  // namespace arduino
}  // namespace athome

#endif /* AENCRYPTEDSTREAM_HPP */
