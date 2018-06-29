#ifndef QUEUE_HPP
# define QUEUE_HPP

# include "AtHomeConfig.h"
# if !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE)
#  include "AList.hpp"

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
            Queue &put(T &data) {
                Queue<T> *next = new Queue<T>(data);
                insert(*next);
                return *next;
            }
            Queue &push_back(T &data) {
                Queue *item = this;
                while (item->_next != nullptr) {
                    item = reinterpret_cast<Queue *>(item->_next);
                }
                return item->put(data);
            }
        };
    }
}

# endif /* !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE) */
#endif /* QUEUE_HPP */
