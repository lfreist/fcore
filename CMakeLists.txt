cmake_minimum_required(VERSION 3.26)
project(fcore VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" PROJECT_IS_TOP_LEVEL)
option(BUILD_TESTS  "Build tests" ${PROJECT_IS_TOP_LEVEL})

include_directories(include/)

add_subdirectory(src)

add_library(fcore SHARED library.cpp)
target_include_directories(fcore PUBLIC include/)

install(DIRECTORY include/hwinfo DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(BUILD_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()