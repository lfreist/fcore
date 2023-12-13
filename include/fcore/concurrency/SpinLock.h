//
// Created by lfreist on 13/12/2023.
//

#pragma once

#include <atomic>

namespace fcore {

/**
 * Basic spin lock class:
 *  A spin lock is a lock that actively waits for the lock to be released before locking it.
 *  Should only be used for simple and fast concurrent object access.
 *
 * Implementation adopted from [cppreference.com](https://en.cppreference.com/w/cpp/atomic/atomic_flag)
 */
class SpinLock {
 public:
  /// Spinning lock: wait for the lock to be released before locking it.
  void lock();

  /// Release the lock
  void unlock();

 private:
  std::atomic_flag _lock = ATOMIC_FLAG_INIT;
};

}  // namespace fcore