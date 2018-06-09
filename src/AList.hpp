#ifndef ALIST_HPP
# define ALIST_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_ITERABLE) && !defined(DISABLE_LIST)
#  include "AIterable.hpp"

namespace athome {
    namespace utility {
        /**
         * Class created for internal use of this library, but not used right now. Could be removed if considered useless.
         */
        template <typename T>
        class AList : public AIterable<T> {
        public:
            // STL is not available on all platform, shared pointers are too dangerous to be allowed
            AList &operator=(const AList &other) = delete;
            AList(const AList &other) = delete;
            ~AList() {
                if (next != nullptr)
                    delete _next;
            }
            virtual AList *next() { return _next; }
            virtual const AList *next() const { return _next; }
            //virtual void append(T &data) {};
        protected:
            //AList(): AIterable<T>(nullptr), _next(nullptr) {}
            AList(AList *next = nullptr, T *data = nullptr): AIterable<T>(data), _next(next) {}
            void append(AList &next) {
                if (_next != nullptr) {
                    next._next = _next;
                }
                _next = &next;
            }

        protected:
            AList<T>    *_next;
        };
    }
}

# endif /* !defined(DISABLE_ITERABLE) && !defined(DISABLE_LIST) */
#endif /* ALIST_HPP */
