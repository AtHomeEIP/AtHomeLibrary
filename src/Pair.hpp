#ifndef PAIR_HPP
#define PAIR_HPP

namespace athome {
namespace utility {
namespace container {
template <typename T, typename U>
class Pair {
 public:
  Pair() {}

  Pair(T a, U b) : first(a), second(b) {}

  Pair(const Pair &other) : first(other.first), second(other.second) {}

  Pair &operator=(const Pair &other) {
    if (&other != this) {
      first = other.first;
      second = other.second;
    }
    return *this;
  }

  ~Pair() {}

 public:
  T first;
  U second;
};
}  // namespace container
}  // namespace utility
}  // namespace athome

#endif /* PAIR_HPP */
