//
// Created by Leon Fang on 2/21/22.
//

#include <cstdlib>
#include <chrono>
#include <cmath>
#include <iostream>
#include "pa_util.h"
#include <time.h>


class duration_cast;

void* PaUtil_AllocateMemory(long size ){
    return malloc(size);
}

void PaUtil_FreeMemory( void *block ){
    free(block);
}

double PaUtil_GetTime( void ){
    time_t now = time(0);
    return (double)now;
}
