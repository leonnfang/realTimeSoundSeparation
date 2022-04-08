cmake_minimum_required(VERSION 3.20)
project(realTimeSoundSeparation)

set(CMAKE_CXX_STANDARD 23)
include_directories(/usr/local/include)
include_directories(./include)
include_directories(model)
include_directories(./model/include)
link_directories(/usr/local/lib)
#link_directories(src/common)
#link_directories(model)

set(model_src_file)
set(model_header_file model/rnnoise.h)

add_executable(realTimeSoundSeparation
        include/record.h

        main.cpp
        src/record.cpp)

add_executable(paex_record
        include/record.h

        ./examples/paex_record.c
        src/record.cpp )


add_executable(ringbuffer
        include/pa_ringbuffer.h
        include/pa_util.h

        src/pa_ringbuffer.c
        src/hostapi/pa_util.cpp
        examples/paex_record_file.c)

add_executable(wire examples/patest_wire.cpp)

#link_libraries(pa_audio portaudio/lib/.libs/libportaudio.a)
target_link_libraries(realTimeSoundSeparation libportaudio.a)
target_link_libraries(paex_record libportaudio.a)
target_link_libraries(ringbuffer libportaudio.a)
target_link_libraries(wire libportaudio.a)