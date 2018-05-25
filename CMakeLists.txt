cmake_minimum_required(VERSION 3.9)

option(SOE_BUILD_TEST "whether or not to build the test" ON)

include(external/external.cmake)

add_executable(static-ordering-eval static-ordering-eval.cpp)
target_link_libraries(static-ordering-eval CLI11::CLI11)
target_compile_features(static-ordering-eval PRIVATE cxx_std_17)

if (SOE_BUILD_TEST)
    set(CMAKE_EXE_LINKER_FLAGS "-pthread")
    find_package(GTest REQUIRED)
    enable_testing()
    include(GoogleTest)
    add_executable(tests tests.cpp)
    target_link_libraries(tests gtest)
    target_compile_features(tests PRIVATE cxx_std_17)
    gtest_add_tests(tests tests.cpp AUTO)
endif()
