cmake_minimum_required(VERSION 3.13)

project(SIR CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-readability-else-after-return)

list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules)
find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(SIR main.cpp board.cpp display.cpp points.cpp graph.cpp)
target_compile_options(SIR PRIVATE -Wall -Wextra -fsanitize=address -O3)
target_include_directories(SIR PRIVATE ${SFML_INCLUDE_DIR})
target_link_options(SIR PRIVATE -fsanitize=address)
target_link_libraries(SIR PRIVATE ${SFML_LIBRARIES})

add_executable(run_points_tests run_tests.cpp points.test.cpp points.cpp board.cpp)
add_test(NAME point_tests COMMAND run_point_tests)

add_executable(run_board_tests run_tests.cpp board.test.cpp points.cpp board.cpp)
add_test(NAME board_tests COMMAND run_board_tests)

