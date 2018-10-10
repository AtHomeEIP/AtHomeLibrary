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
  Queue() {}
  Queue(T &data) : _data(data) {}
  Queue(const Queue &) = delete;
  Queue &operator=(const Queue &) = delete;
  ~Queue() {
    if (_next != nullptr) {
      delete _next;
    }
  }
  T &get() { return _data; }
  T const &get() const { return _data; }
  void set(T &data) { _data = data; }
  Queue *next() { return _next; }
  const Queue *next() const { return _next; }
  void insert(Queue &next) {
    if (_next != nullptr) {
      next._next = _next;
    }
    _next = &next;
  }
  Queue &put(T &data) {
    Queue *next = new Queue(data);
    insert(*next);
    return *next;
  }
  Queue &push_back(T &data) {
    Queue *item = this;
    while (item->_next != nullptr) {
      item = item->_next;
    }
    return item->put(data);
  }

 private:
  Queue *_next;
  T &_data;
};

template <typename T>
class ConstQueue {
 public:
  ConstQueue() {}
  ConstQueue(T const &data) : _data(data) {}
  ConstQueue(const ConstQueue &) = delete;
  ConstQueue &operator=(const ConstQueue &) = delete;
  ~ConstQueue() {
    if (_next != nullptr) {
      delete _next;
    }
  }
  T const &get() const { return _data; }
  void set(T const &data) { _data = data; }
  ConstQueue *next() { return _next; }
  const ConstQueue *next() const { return _next; }
  void insert(ConstQueue &next) {
    if (_next != nullptr) {
      next._next = _next;
    }
    _next = &next;
  }
  ConstQueue &put(T const &data) {
    ConstQueue *next = new ConstQueue(data);
    insert(*next);
    return *next;
  }
  ConstQueue &push_back(T const &data) {
    ConstQueue *item = this;
    while (item->_next != nullptr) {
      item = item->_next;
    }
    return item->put(data);
  }

 private:
  ConstQueue *_next;
  T const &_data;
};
}  // namespace utility
}  // namespace athome

#endif /* !defined(DISABLE_LIST) && !defined(DISABLE_QUEUE) */
#endif /* QUEUE_HPP */
