#ifndef ASMARTPTR_HPP_
#define ASMARTPTR_HPP_

namespace athome {
namespace utility {
namespace memory {
template <typename T>
class ASmartPtr {
 public:
  ASmartPtr() {}

  ASmartPtr(const ASmartPtr &other) {}

  ASmartPtr &operator=(const ASmartPtr &other) = delete;

  virtual ~ASmartPtr() {}

  void reset(T *ptr = nullptr) {
    unset();
    if (ptr != nullptr) {
      set(ptr);
    }
  }

  void swap(ASmartPtr &other) {
    T *tmp = other.m_value;
    other.set(m_value);
    set(tmp);
  }

  T *get() const { return m_value; }

  T &operator*() const {
    if (m_value == nullptr) {
      // implement exception or default value
    }
    return *m_value;
  }

  T *operator->() const { return get(); }

  operator bool() const { return m_value != nullptr; }

  friend void swap(ASmartPtr &a, ASmartPtr &b) { a.swap(b); }

 protected:
  virtual void set(T *ptr) = 0;

  virtual void unset() = 0;

  void copy(const ASmartPtr &other) {
    if (m_value != nullptr) {
      unset();
    }
    set(other.get());
  }

 protected:
  T *m_value;
};
}  // namespace memory
}  // namespace utility
}  // namespace athome

#endif  // ASMARTPTR_HPP_
