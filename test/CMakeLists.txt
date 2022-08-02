cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(opendsa-test VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(INSTALL_GMOCK "Install Googletest's GMock?" OFF)
option(INSTALL_GTEST "Install Googletest's GTest?" OFF)

enable_testing()

# Fetch Google Test Suite
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(vector_test vector.cc)
target_link_libraries(vector_test   GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(vector_test)

add_executable(deque_test deque.cc)
target_link_libraries(deque_test   GTest::gtest_main)
gtest_discover_tests(deque_test)

add_custom_target(test COMMAND ctest )
