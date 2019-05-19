#ifndef ATHOMEMODULEUTIL_HPP
#define ATHOMEMODULEUTIL_HPP

namespace athome {
namespace module {
  template <typename U>
  inline bool is_non_zero(const U &data) {
    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&data);
    for (size_t i = 0; i < sizeof(U); i++) {
      if (*ptr++ & 0xFF) {
        return true;
      }
    }
    return false;
  }

  template <typename U>
  inline bool is_zero(const U &data) {
    return !is_non_zero<U>(data);
  }

  /**
   * Compute the encoded length of string encoded in base64
   */
  static constexpr size_t base64_encoded_length(size_t plain_length) {
    return ((plain_length + 2) / 3 * 4) + 1;
  }

  template <typename U>
  static constexpr size_t base64_encoded_length() {
      return base64_encoded_length(sizeof(U));
  }
}
}

#endif  // !ATHOMEMODULEUTIL_HPP
