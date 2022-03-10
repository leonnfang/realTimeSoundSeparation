//
// Created by leonfang on 3/1/22.
//

#ifndef REALTIMESOUNDSEPARATION_NNET_H
#define REALTIMESOUNDSEPARATION_NNET_H


#include <stdlib.h>
#include <math.h>
#include "opus_types.h"
#include "arch.h"
#include "common.h"
#include "tansig_table.h"
#include "nnet.h"
#include "nnet_data.h"

#define SOFTMAX_HACK

#ifdef __AVX__
#include "vec_avx.h"
#elif __ARM_NEON__
#include "vec_neon.h"
#else
#warning Compiling without any vectorization. This code will be very slow
#include "vec.h"
#endif

static OPUS_INLINE float relu(float x);


static void sgemv_accum(float *out, const float *weights, int rows, int cols, int col_stride, const float *x);

void compute_activation(float *output, float *input, int N, int activation);

void compute_dense(const DenseLayer *layer, float *output, const float *input);

void compute_gru(const GRULayer *gru, float *state, const float *input);

void compute_conv1d(const Conv1DLayer *layer, float *output, float *mem, const float *input);

void compute_embedding(const EmbeddingLayer *layer, float *output, int input);

void accum_embedding(const EmbeddingLayer *layer, float *output, int input);



#endif //REALTIMESOUNDSEPARATION_NNET_H
