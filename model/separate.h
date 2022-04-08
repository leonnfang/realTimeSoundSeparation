#ifndef TEST_H
#define TEST_H
#include <stdio.h>
#include "rnnoise.h"
#include <iostream>
#include "separate.h"
#define FRAME_SIZE 480

int separate(short *input, short *output, DenoiseState *st);

#endif