cmake_minimum_required(VERSION 3.13)

project(SIR CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-readability-else-after-return)

list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules)
find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(SIR source/main.cpp source/board.cpp source/display.cpp source/points.cpp source/graph.cpp source/info.cpp)
target_compile_options(SIR PRIVATE -Wall -Wextra -fsanitize=address -O3)
target_include_directories(SIR PRIVATE ${SFML_INCLUDE_DIR})
target_link_options(SIR PRIVATE -fsanitize=address)
target_link_libraries(SIR PRIVATE ${SFML_LIBRARIES})

add_executable(run_points_tests test/run_tests.cpp test/points.test.cpp source/points.cpp source/board.cpp)
add_test(NAME point_tests COMMAND run_point_tests)

add_executable(run_board_tests test/run_tests.cpp test/board.test.cpp source/points.cpp source/board.cpp)
add_test(NAME board_tests COMMAND run_board_tests)
