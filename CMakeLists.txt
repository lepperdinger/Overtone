cmake_minimum_required(VERSION 3.16)
project(Overtone)
project(tests)

include_directories(src)

file(GLOB SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(Overtone
               apps/Overtone.cpp
               ${SRC})

add_executable(tests
               apps/tests.cpp
               ${SRC})

target_compile_options(Overtone PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
