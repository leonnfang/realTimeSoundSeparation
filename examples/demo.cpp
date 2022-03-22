//
// Created by Leon Fang on 3/13/22.
//
#include "rnn.h"

int main(){
    RNNState* temp = new RNNState;
    GRULayer* tempLayer = new GRULayer;
//    temp->model->gru1 = tempLayer;
    float number = 1.0f;
    compute_rnn(temp,&number,&number,&number);
    return 0;
}
