add_library(concurrency SHARED SpinLock.cpp)
add_library(concurrency_static STATIC SpinLock.cpp)

add_library(fcore::concurrency ALIAS concurrency)
add_library(fcore::concurrency_static ALIAS concurrency_static)