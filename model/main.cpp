#include <stdio.h>
#include "rnnoise.h"
#include "separate.h"
#include <iostream>
#define FRAME_SIZE 480

//using namespace std;
#ifndef TRAINING
#define TRAINING 0
#endif

int main(int argc, char **argv)
{
  if(TRAINING){
    train(argc, argv);
    return 0;
  }
  int i;
  int first = 1;
  float x[FRAME_SIZE];
  FILE *f1, *fout, *f_feature;
  DenoiseState *st;
  st = rnnoise_create(NULL);
  if (argc!=3) {
    fprintf(stderr, "usage: %s <noisy speech> <output denoised>\n", argv[0]);
    return 1;
  }
  // std::cout << *st << std::endl;
  f1 = fopen(argv[1], "rb");
  fout = fopen(argv[2], "wb");
  f_feature = fopen("feature_test.raw", "wb");
  while (1) {
    short tmp[FRAME_SIZE]; 
    fread(tmp, sizeof(short), FRAME_SIZE, f1);
    if (feof(f1)) break;
    // for (i=0;i<FRAME_SIZE;i++) x[i] = ((float)tmp[i])/32768.f;
    // rnnoise_process_frame(st, x, x, f_feature);
    // for (i=0;i<FRAME_SIZE;i++) tmp[i] = x[i]*32768;
    // std::cout << *x << std::endl;
    separate(tmp, tmp, st);
    if (!first) fwrite(tmp, sizeof(short), FRAME_SIZE, fout);
    first = 0;
  }
  // rnnoise_destroy(st);
  fclose(f1);
  fclose(fout);
  return 0;
}