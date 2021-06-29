cmake_minimum_required(VERSION 3.12)

add_executable(opendsa_sorting_test)
target_sources(
  opendsa_sorting_test 
  PRIVATE 
    sorting/bubble_sort.cc
    sorting/insertion_sort.cc
    sorting/selection_sort.cc
    sorting/quick_sort.cc
    sorting/merge_sort.cc
    sorting/counting_sort.cc
    )

target_include_directories(
  opendsa_sorting_test PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(opendsa_sorting_test PRIVATE cxx_std_17)
target_link_libraries(opendsa_sorting_test gtest_main gmock_main)

target_compile_options(
  opendsa_sorting_test 
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Werror>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-pedantic>
)

add_executable(opendsa_container_test)
target_sources(
  opendsa_container_test
  PRIVATE 
    container/init.cc
    )

target_include_directories(
  opendsa_container_test PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

target_compile_features(opendsa_container_test PRIVATE cxx_std_17)
target_link_libraries(opendsa_container_test gtest_main gmock_main)

target_compile_options(
  opendsa_container_test 
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/std:c++17>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Werror>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-pedantic>
)


add_test(NAME opendsa_sorting_test COMMAND opendsa_sorting_test)
add_test(NAME opendsa_container_test COMMAND opendsa_container_test)