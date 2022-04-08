#include <stdio.h>
#include "rnnoise.h"
#include <iostream>
#define FRAME_SIZE 480

//using namespace std;
#ifndef TRAINING
#define TRAINING 0
#endif

int test(short *input, short *output, DenoiseState *st)
{
  int i;
  int first = 1;
  float x[FRAME_SIZE];
  FILE *f_feature;
  // DenoiseState *st;
  // st = rnnoise_create(NULL);
  f_feature = fopen("feature_test.raw", "wb");
  // std::cout << *input << std::endl;
  for (i=0;i<FRAME_SIZE;i++) x[i] = ((float)input[i])/32768.f;
  // std::cout << *x << std::endl;
  rnnoise_process_frame(st, x, x, f_feature);
  // std::cout << *x << std::endl;
  for (i=0;i<FRAME_SIZE;i++) output[i] = x[i]*32768;
  // std::cout << *output << std::endl;
  first = 0;
  // rnnoise_destroy(st);
  fclose(f_feature);
  return 0;
}

