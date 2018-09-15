#ifndef WEAKPTR_HPP_
#define WEAKPTR_HPP_
#include "SharedPtr.hpp"

namespace athome {
namespace utility {
namespace memory {
template <typename T>
class WeakPtr : public ASharedPtr<T> {
 public:
  WeakPtr(T *ptr = nullptr) : ASharedPtr<T>() {
    if (ptr != nullptr) {
      set(ptr);
    } else {
      ASmartPtr<T>::m_value = nullptr;
    }
  }

  WeakPtr(const WeakPtr &other) : ASharedPtr<T>(other) { copy(other); }

  WeakPtr &operator=(const SharedPtr &other) {
    copy(other);
    return *this;
  }

  virtual ~WeakPtr() {}

  bool expired() const { return !ASharedPtr<T>::m_pointers[m_value]; }

  SharedPtr<T> lock() const {
    SharedPtr<T> ptr(ASmartPtr<T>::m_value);
    return ptr;
  }

 protected:
  virtual void set(T *ptr) { ASmartPtr<T>::m_value = ptr; }

  virtual void unset() { ASmartPtr<T>::m_value = nullptr; }
};
}  // namespace memory
}  // namespace utility
}  // namespace athome
#endif  // WEAKPTR_HPP_
