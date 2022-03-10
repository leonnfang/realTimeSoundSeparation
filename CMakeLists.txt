cmake_minimum_required(VERSION 3.20)
project(realTimeSoundSeparation)

set(CMAKE_CXX_STANDARD 23)
include_directories(/usr/local/include)
include_directories(./include)
include_directories(./model/include)
link_directories(/usr/local/lib)
link_directories(src/common)
add_executable(realTimeSoundSeparation main.cpp
        include/record.h
        model/include/rnn.h
        src/record.cpp)
add_executable(paex_record ./examples/paex_record.c
        include/record.h
        model/include/rnn.h

        src/record.cpp )
add_executable(ocean_shore ./examples/paex_ocean_shore.c
        include/pa_ringbuffer.h
        include/pa_util.h
        include/pa_memorybarrier.h
        include/pa_hostapi.h
        include/pa_stream.h
        include/pa_mac_core.h
        include/pa_mac_core_blocking.h
        include/pa_mac_core_internal.h
        include/pa_mac_core_utilities.h
        model/include/rnn.h

        src/pa_ringbuffer.c
        src/hostapi/pa_util.cpp
        model/src/rnn.cpp)
#link_libraries(pa_audio portaudio/lib/.libs/libportaudio.a)
target_link_libraries(realTimeSoundSeparation libportaudio.a)
target_link_libraries(paex_record libportaudio.a)
target_link_libraries(ocean_shore libportaudio.a)