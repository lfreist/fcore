//
// Created by lfreist on 13/12/2023.
//

#include <fcore/concurrency/SpinLock.h>

namespace fcore {

// --- SpinLock --------------------------------------------------------------------------------------------------------
// _____________________________________________________________________________________________________________________
void SpinLock::lock() {
  while (_lock.test_and_set(std::memory_order_acquire)) {
    // spin
  }
}

// _____________________________________________________________________________________________________________________
void SpinLock::unlock() { _lock.clear(std::memory_order_release); }

}  // namespace fcore