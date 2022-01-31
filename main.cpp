#include <iostream>
#include <portaudio.h>
PaError err = Pa_Initialize();
const char* error_message = Pa_GetErrorText(err);
int main() {
    std::cout << "Hello, World!" << std::endl;

    // terminate the pa
    err = Pa_Terminate();
    if( err != paNoError )
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    return 0;
}
