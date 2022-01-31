cmake_minimum_required(VERSION 3.20)
project(realTimeSoundSeparation)

set(CMAKE_CXX_STANDARD 23)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
add_executable(realTimeSoundSeparation main.cpp)
#link_libraries(pa_audio portaudio/lib/.libs/libportaudio.a)
target_link_libraries(realTimeSoundSeparation libportaudio.a)