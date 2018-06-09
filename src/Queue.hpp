#ifndef QUEUE_HPP
# define QUEUE_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE)
#  include "AList.hpp"

// Todo: Recursive algorithms aren't great on embedded platforms :(

namespace athome {
    namespace utility {
        /**
         * Class created for internal use of this library, but not used right now. Could be removed if considered useless.
         */
        template <typename T>
        class Queue : public AList<T> {
        public:
            Queue():AList<T>() {}
            Queue(T &data):AList<T>(&data) {}
            Queue(const Queue &) = delete;
            Queue &operator=(const Queue &) = delete;
            Queue &append(T &data) {
                if (this->_next == nullptr) {
                    Queue<T> *next = new Queue<T>(data);
                    append(*next);
                    return *next;
                }
                else {
                    return this->_next->append(data);
                }
            }
        };
    }
}

# endif /* !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE) */
#endif /* QUEUE_HPP */
