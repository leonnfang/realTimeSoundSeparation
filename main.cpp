#include <iostream>
#include <portaudio.h>
#include "./include/record.h"

//PaError err = Pa_Initialize();
//const char* error_message = Pa_GetErrorText(err);
int main() {
    std::cout << "Hello, World!" << std::endl;
    record();
    return 0;
}
