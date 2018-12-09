#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "AtHomeConfig.h"
#if !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE)

namespace athome {
namespace utility {
/**
 * Class created for internal use of this library, but not used right now. Could
 * be removed if considered useless.
 */
template <typename T>
class Queue {
 public:
  Queue():_next(nullptr), _data(nullptr) {}
  Queue(T &data):_next(nullptr), _data(&data) {}
  Queue(const Queue &) = delete;
  Queue &operator=(const Queue &) = delete;
  ~Queue() {
    if (_next != nullptr) {
      delete _next;
    }
  }
  T *get() { return _data; }
  T const *get() const { return _data; }
  void set(T &data) { _data = &data; }
  Queue *next() { return _next; }
  const Queue *next() const { return _next; }
  void insert(Queue &next) {
    if (_next != nullptr) {
      next._next = _next;
    }
    _next = &next;
  }
  Queue *put(T &data) {
    Queue *next = new Queue(data);
    insert(*next);
    return next;
  }
  Queue *push_back(T &data) {
    Queue *item = this;
    while (item->_next != nullptr) {
      item = item->_next;
    }
    return item->put(data);
  }

 private:
  Queue *_next;
  T *_data;
};
}  // namespace utility
}  // namespace athome

#endif /* !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE) */
#endif /* QUEUE_HPP */
