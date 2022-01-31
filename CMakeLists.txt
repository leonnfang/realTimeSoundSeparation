cmake_minimum_required(VERSION 3.20)
project(realTimeSoundSeparation)

set(CMAKE_CXX_STANDARD 23)

#include_directories(portaudio/include)
#include_directories(portaudio/lib)
add_executable(realTimeSoundSeparation main.cpp)
#link_libraries(pa_audio portaudio/lib/.libs/libportaudio.a)
#target_link_libraries(realTimeSoundSeparation ${pa_audio})