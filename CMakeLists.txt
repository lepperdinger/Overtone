cmake_minimum_required(VERSION 3.16)
project(Overtone)
project(tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
