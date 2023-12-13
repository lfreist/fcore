//
// Created by lfreist on 13/12/2023.
//

#pragma once

#include <vector>
#include <thread>
#include <functional>

#include <fcore/concurrency/ThreadSafeQueue.h>

namespace fcore {

class ThreadPool {
 public:
  ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) : _stop(false), _threads(num_threads) {
    for (auto& thread : _threads) {}
  }
 private:
  std::vector<std::thread> _threads;
  ThreadSafeQueue<std::function<void()>> _task_queue;
  std::atomic<bool> _stop;
};

}