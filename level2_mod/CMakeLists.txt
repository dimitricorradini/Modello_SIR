cmake_minimum_required(VERSION 3.10)

project(SIR CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,-readability-else-after-return)

list(APPEND CMAKE_MODULE_PATH /usr/share/SFML/cmake/Modules)
find_package(SFML COMPONENTS graphics window system REQUIRED)

add_executable(SIR main_mod.cpp board_mod.cpp display_mod.cpp points_mod.cpp)
target_compile_options(SIR PRIVATE -Wall -Wextra -fsanitize=address -O3)
target_include_directories(SIR PRIVATE ${SFML_INCLUDE_DIR})
target_link_options(SIR PRIVATE -fsanitize=address)
target_link_libraries(SIR PRIVATE ${SFML_LIBRARIES})
