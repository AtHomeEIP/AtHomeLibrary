#ifndef SHAREDPTR_HPP_
#define SHAREDPTR_HPP_
#include "ASharedPtr.hpp"

namespace athome {
namespace utility {
namespace memory {
template <typename T>
class SharedPtr : public ASharedPtr<T> {
 public:
  SharedPtr(T *ptr = nullptr) : ASharedPtr<T>() {
    if (ptr != nullptr) {
      set(ptr);
    } else {
      ASmartPtr<T>::m_value = nullptr;
    }
  }

  SharedPtr(const SharedPtr &other) : ASharedPtr<T>(other) { copy(other); }

  SharedPtr &operator=(const SharedPtr &other) {
    copy(other);
    return *this;
  }

  virtual ~SharedPtr() { unset(); }

 protected:
  virtual void set(T *ptr) {
    if (ASmartPtr<T>::m_value != nullptr) {
      unset();
    }
    ASmartPtr<T>::m_value = ptr;
    ASharedPtr<T>::m_pointers[ptr] = ASharedPtr<T>::m_pointers[ptr] + 1;
  }

  virtual void unset() {
    size_t count = ASharedPtr<T>::m_pointers[ASmartPtr<T>::m_value];
    if (count) {
      ASharedPtr<T>::m_pointers[ASmartPtr<T>::m_value] = count - 1;
    } else {
      delete ASmartPtr<T>::m_value;
    }
    ASmartPtr<T>::m_value = nullptr;
  }
};
}  // namespace memory
}  // namespace utility
}  // namespace athome

#endif  // SHAREDPTR_HPP_
