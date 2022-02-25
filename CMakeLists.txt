cmake_minimum_required(VERSION 3.16)
project(Overtone)
project(tests)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

#add_compile_definitions(DEBUG)

file(GLOB SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_executable(Overtone
               apps/Overtone.cpp
               ${SRC})

add_executable(tests
               apps/tests.cpp
               ${SRC})

if(MSVC)
  target_compile_options(Overtone PRIVATE /W4 /WX)
  target_compile_options(tests PRIVATE /W4 /WX)
else()
  target_compile_options(Overtone PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
