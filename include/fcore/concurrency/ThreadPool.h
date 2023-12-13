//
// Created by lfreist on 13/12/2023.
//

#pragma once

#include <fcore/concurrency/ThreadSafeQueue.h>

namespace fcore {

template <typename Q = ThreadSafeQueue>
class ThreadPool {};

}