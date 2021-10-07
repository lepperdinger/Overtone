cmake_minimum_required(VERSION 3.16)
project(Overtone)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

#add_compile_definitions(DEBUG)

add_executable(Overtone
               src/OvertoneApp.cpp
               src/OvertoneApp.h
               src/FFmpeg.cpp
               src/FFmpeg.h
               src/WAVE.cpp
               src/WAVE.h
               src/JSON.cpp
               src/JSON.h
               src/Spectrum.cpp
               src/Spectrum.h
               src/Keyboard.cpp
               src/Keyboard.h
               src/VideoFrame.cpp
               src/VideoFrame.h)
