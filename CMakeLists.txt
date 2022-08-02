cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(opendsa VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ignore testing option
option(BUILD_TEST "Build the unit tests" ON)

option(INSTALL_GMOCK "Install Googletest's GMock?" OFF)
option(INSTALL_GTEST "Install Googletest's GTest?" OFF)

# include tests
if (BUILD_TEST)
    add_subdirectory(test)
endif()

# add include directory for headers
include_directories(include)

# add the executables
add_executable(main main.cpp)           # Mainly for testing local
add_executable(vector driver/vector.cpp)
add_executable(deque driver/deque.cpp)
add_executable(queue driver/queue.cpp)
add_executable(sgl_list driver/sgl_list.cpp)

# install headers to local machines
install(FILES include/deque.h DESTINATION include/opendsa RENAME deque)
install(FILES include/vector.h DESTINATION include/opendsa RENAME vector)
install(FILES include/singly_list.h DESTINATION include/opendsa RENAME singly_list)
install(FILES include/queue.h DESTINATION include/opendsa RENAME queue)
install(FILES include/algorithm.h DESTINATION include/opendsa RENAME algorithm)
install(FILES include/iterator.h DESTINATION include/opendsa RENAME iterator)
install(FILES include/helper.h DESTINATION include/opendsa RENAME helper)
