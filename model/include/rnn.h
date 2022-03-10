//
// Created by Leon Fang on 3/9/22.
//

#ifndef REALTIMESOUNDSEPARATION_RNN_H
#define REALTIMESOUNDSEPARATION_RNN_H

#include "nnet_data.h"

void compute_rnn(RNNState *rnn, float *gains, float *strengths, const float *input);


#endif //REALTIMESOUNDSEPARATION_RNN_H
