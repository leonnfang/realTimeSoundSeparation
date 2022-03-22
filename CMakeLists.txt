cmake_minimum_required(VERSION 3.20)
project(realTimeSoundSeparation)

set(CMAKE_CXX_STANDARD 23)
include_directories(/usr/local/include)
include_directories(./include)
include_directories(./model/include)
link_directories(/usr/local/lib)
#link_directories(src/common)
#link_directories(model)

set(model_src_file)
set(model_header_file)
add_executable(demo examples/demo.cpp
        model/src/rnn.cpp
        model/src/nnet.cpp

        model/include/rnn.h
        model/include/nnet.h)

add_executable(realTimeSoundSeparation
        include/record.h
        model/include/rnn.h

        main.cpp
        src/record.cpp)
add_executable(paex_record
        include/record.h
        model/include/rnn.h

        ./examples/paex_record.c
        src/record.cpp )
add_executable(ocean_shore
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
        model/include/nnet.h
        model/include/vec.h

        ./examples/paex_ocean_shore.c
        src/pa_ringbuffer.c
        src/hostapi/pa_util.cpp
        model/src/nnet.cpp
        model/src/rnn.cpp)
#link_libraries(pa_audio portaudio/lib/.libs/libportaudio.a)
target_link_libraries(realTimeSoundSeparation libportaudio.a)
target_link_libraries(paex_record libportaudio.a)
target_link_libraries(ocean_shore libportaudio.a)