cmake_minimum_required(VERSION 3.16)
project(Overtone)

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

target_compile_options(Overtone PRIVATE -Wall -Wextra -Wpedantic -Werror)

enable_testing()

add_executable(test_LinearInterpolation test/test_LinearInterpolation.cpp ${SRC})
target_link_libraries(test_LinearInterpolation gtest gtest_main)
add_test(test_LinearInterpolation test_LinearInterpolation)

add_executable(test_KeyboardFrequencies test/test_KeyboardFrequencies.cpp ${SRC})
target_link_libraries(test_KeyboardFrequencies gtest gtest_main)
add_test(test_KeyboardFrequencies test_KeyboardFrequencies)

add_executable(test_RGBColor test/test_RGBColor.cpp ${SRC})
target_link_libraries(test_RGBColor gtest gtest_main)
add_test(test_RGBColor test_RGBColor)
