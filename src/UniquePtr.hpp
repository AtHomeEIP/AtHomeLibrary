#ifndef UNIQUEPTR_HPP_
#define UNIQUEPTR_HPP_
#include <algorithm>
#include <utility>
#include "ASmartPtr.hpp"

namespace athome {
namespace utility {
namespace memory {
template <typename T>
class UniquePtr : ASmartPtr<T> {
 public:
  UniquePtr(T *ptr = nullptr) : ASmartPtr<T>() {
    if (ptr != nullptr) {
      set(ptr)
    } else {
      ASmartPtr<T>::m_value = nullptr;
    }
  }

  UniquePtr(UniquePtr &other) { takePtr(other); }

  UniquePtr &operator=(UniquePtr &other) {
    if (&other != this) {
      takePtr(other);
    }
    return *this;
  }

  UniquePtr &operator=(T *ptr) {
    set(ptr);
    return *this;
  }

  ~UniquePtr() { unset(ptr); }

  T *release() {
    T *ptr = ASmartPtr<T>::m_value;
    ASmartPtr<T>::m_value = nullptr;
    return ptr;
  }

 protected:
  void takePtr(UniquePtr &other) {
    set(other.m_value);
    other.m_value = nullptr;
  }

  virtual void set(T *ptr) {
    if (ASmartPtr<T>::m_value != nullptr) {
      unset();
    }
    ASmartPtr<T>::m_value = ptr;
  }

  virtual void unset() {
    if (ASmartPtr<T>::m_value != nullptr) {
      delete ASmartPtr<T>::m_value;
    }
    ASmartPtr<T>::m_value = nullptr;
  }
};
}  // namespace memory
}  // namespace utility
}  // namespace athome

#endif  // UNIQUEPTR_HPP_
