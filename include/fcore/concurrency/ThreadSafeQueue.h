//
// Created by lfreist on 13/12/2023.
//

#pragma once

#include <condition_variable>
#include <exception>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>

namespace fcore {

class PushToClosedQueueException : public std::exception {
 public:
  PushToClosedQueueException() : _message("Push to closed queue is prohibited.") {}

  const char* what() const override {
    return _message.c_str();
  }

 private:
  std::string _message;
};

/**
 * A thread safe queue built on top of std::queue (multi consumer, multi producer).
 *
 * @tparam T
 */
template <class T>
class ThreadSafeQueue {
 public:
  explicit ThreadSafeQueue(unsigned maxSize = 100) {
    _maxSize = maxSize;
  }

  /**
   * Push element to queue.
   *
   * @param element
   * @return false if queue is closed else true
   */
  void push(T element) {
    if (isClosed()) {
      throw PushToClosedQueueException();
    }
    std::unique_lock lockQueue(_queueMutex);
    while (_queue.size() >= _maxSize) {
      _pushCondVar.wait(lockQueue);
    }
    _queue.push(std::move(element));
    _popCondVar.notify_one();
  }

  /**
   * Push element to queue. If the queue is at minimum size of maxSize / 2,
   * push_warn_flag is set to true. If queue is at maxSize and allow_oversize
   * is true, data are pushed anyways!
   *
   * @param element
   * @param push_warn_flag
   * @param allow_oversize
   */
  void push(T element, bool* push_warn_flag, bool allow_oversize = false) {
    if (isClosed()) {
      throw PushToClosedQueueException();
    }
    std::unique_lock lockQueue(_queueMutex);
    if (!allow_oversize) {
      while (_queue.size() >= _maxSize) {
        _pushCondVar.wait(lockQueue);
      }
    }
    _queue.push(std::move(element));
    *push_warn_flag = (_queue.size() >= (_maxSize / 2));
    _popCondVar.notify_one();
  }

  /**
   * Pop an element from queue. Note that in contrast to std::queue, this
   * pop returns the first element. If the queue is empty, pop() waits for a
   * push() call. If the queue is empty and the queue was closed, pop() returns
   * empty optional.
   *  In contrast to std::queue::pop, this pop returns the element.
   *
   * @return T
   */
  std::optional<T> pop() {
    std::unique_lock lockQueue(_queueMutex);
    while (_queue.empty()) {
      if (isClosed()) {
        return {};
      }
      _popCondVar.wait(lockQueue);
    }
    T element = std::move(_queue.front());
    _queue.pop();
    _pushCondVar.notify_one();
    return element;
  }

  /**
   * Pop element from queue. If queue is empty but not closed, pop_failed_flag
   * is set to true.
   *  In contrast to std::queue::pop, this pop returns the element.
   *
   * @param pop_failed_flag
   * @return
   */
  std::optional<T> pop(bool* pop_failed_flag) {
    std::unique_lock lockQueue(_queueMutex);
    if (_queue.empty()) {
      if (isClosed()) {
        *pop_failed_flag = false;
        return {};
      }
      *pop_failed_flag = true;
      return {};
    }
    T element = std::move(_queue.front());
    _queue.pop();
    _pushCondVar.notify_one();
    *pop_failed_flag = false;
    return element;
  }

  /**
   * Returns, whether the Queue is empty or not
   *
   * @return bool
   */
  bool empty() const {
    std::lock_guard lockQueue(_queueMutex);
    return _queue.empty();
  }

  /**
   * Returns number of elements in the queue
   *
   * @return size_t
   */
  size_t size() const {
    std::lock_guard lockQueue(_queueMutex);
    return _queue.size();
  }

  /**
   * Closes the queue -> no pushes possible anymore
   */
  void close() {
    if (isClosed()) {
      return;
    }
    _closed.store(true);
    _popCondVar.notify_all();
  }

  bool isClosed() {
    return _closed.load();
  }

  void reset() {
    _closed.store(false);
    _queue = std::queue<T>();
  }

  bool isFull() { return size() >= _maxSize; }

 private:
  unsigned _maxSize;
  std::atomic<bool> _closed = false;

  std::queue<T> _queue;

  mutable std::mutex _queueMutex;

  std::condition_variable _popCondVar;
  std::condition_variable _pushCondVar;
};

class OrderedThreadSafeQueue {};

}