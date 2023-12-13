//
// Created by lfreist on 13/12/2023.
//

#pragma once

#include <concepts>

namespace fcore {

    template <typename T>
    concept Lockable = requires(T t) {
        { t.lock() } -> std::same_as<void>;
        { t.unlock() } -> std::same_as<void>;
    };

    template <typename T>
    concept SharedLockable = requires(T t) {
        { t.lock_shared() } -> std::same_as<void>;
        { t.unlock_shared() } -> std::same_as<void>;
    };

}