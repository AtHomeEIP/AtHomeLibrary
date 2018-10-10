#ifndef ASHAREDPTR_HPP_
#define ASHAREDPTR_HPP_
#include <map>
#include "ASmartPtr.hpp"

namespace athome {
namespace utility {
namespace memory {
template <typename T>
class ASharedPtr : public ASmartPtr<T> {
 public:
  ASharedPtr() : ASmartPtr<T>() {}

  ASharedPtr(const ASharedPtr &other) : ASmartPtr<T>(other) {}

  ASharedPtr &operator=(const ASharedPtr &other) = delete;

  virtual ~ASharedPtr() {}

  long use_count() const { return m_pointers[ASmartPtr<T>::m_value]; }

  bool unique() const { return use_count() == 1; }

 protected:
  virtual void set(T *ptr) = 0;

  virtual void unset() = 0;

 private:
  static std::map<void *, size_t> m_pointers;
};

template <typename T>
std::map<void *, size_t> ASharedPtr<T>::m_pointers;
}  // namespace memory
}  // namespace utility
}  // namespace athome

#endif  // ASHAREDPTR_HPP_
