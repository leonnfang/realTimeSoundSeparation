/** @file patest_wire.c
    @ingroup test_src
    @brief Pass input directly to output.

    Note that some HW devices, for example many ISA audio cards
    on PCs, do NOT support full duplex! For a PC, you normally need
    a PCI based audio card such as the SBLive.

    @author Phil Burk  http://www.softsynth.com

 While adapting to V19-API, I excluded configs with framesPerCallback=0
 because of an assert in file pa_common/pa_process.c. Pieter, Oct 9, 2003.

*/


#include <stdio.h>
#include <math.h>
#include <vector>
#include "portaudio.h"
#include "separate.h"

#define SAMPLE_RATE            (48000)

typedef struct WireConfig_s {
    int isInputInterleaved;
    int isOutputInterleaved;
    int numInputChannels;
    int numOutputChannels;
    int framesPerCallback;
    /* count status flags */
    int numInputUnderflows;
    int numInputOverflows;
    int numOutputUnderflows;
    int numOutputOverflows;
    int numPrimingOutputs;
    int numCallbacks;
} WireConfig_t;

#define USE_FLOAT_INPUT        (0)
#define USE_FLOAT_OUTPUT       (0)

/* Latencies set to defaults. */

#if USE_FLOAT_INPUT
#define INPUT_FORMAT  paFloat32
typedef float INPUT_SAMPLE;
#else
#define INPUT_FORMAT  paInt16
typedef short INPUT_SAMPLE;
#endif

#if USE_FLOAT_OUTPUT
#define OUTPUT_FORMAT  paFloat32
typedef float OUTPUT_SAMPLE;
#else
#define OUTPUT_FORMAT  paInt16
typedef short OUTPUT_SAMPLE;
#endif

double gInOutScaler = 1.0;
#define CONVERT_IN_TO_OUT(in) ((OUTPUT_SAMPLE) ((in) * 1))

#define INPUT_DEVICE           (Pa_GetDefaultInputDevice())
#define OUTPUT_DEVICE          (Pa_GetDefaultOutputDevice())

INPUT_SAMPLE tmpInput[480];
OUTPUT_SAMPLE tmpOutput[480];
//int inputIndex = 0;
DenoiseState* st = rnnoise_create(NULL);

static PaError TestConfiguration(WireConfig_t *config);

static int wireCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData);

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/

static int wireCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
    INPUT_SAMPLE *in;
    OUTPUT_SAMPLE *out;
    int inStride;
    int outStride;
    int inDone = 0;
    int outDone = 0;
    WireConfig_t *config = (WireConfig_t *) userData;
    unsigned int i;
    int inChannel, outChannel;

    /* This may get called with NULL inputBuffer during initial setup. */
    if (inputBuffer == NULL) return 0;

    /* Count flags */
    if ((statusFlags & paInputUnderflow) != 0) config->numInputUnderflows += 1;
    if ((statusFlags & paInputOverflow) != 0) config->numInputOverflows += 1;
    if ((statusFlags & paOutputUnderflow) != 0) config->numOutputUnderflows += 1;
    if ((statusFlags & paOutputOverflow) != 0) config->numOutputOverflows += 1;
    if ((statusFlags & paPrimingOutput) != 0) config->numPrimingOutputs += 1;
    config->numCallbacks += 1;

    inChannel = 0, outChannel = 0;
    while (!(inDone && outDone)) {
        if (config->isInputInterleaved) {
            in = ((INPUT_SAMPLE *) inputBuffer) + inChannel;
            inStride = config->numInputChannels;
        } else {
            in = ((INPUT_SAMPLE **) inputBuffer)[inChannel];
            inStride = 1;
        }

        if (config->isOutputInterleaved) {
            out = ((OUTPUT_SAMPLE *) outputBuffer) + outChannel;
            outStride = config->numOutputChannels;
        } else {
            out = ((OUTPUT_SAMPLE **) outputBuffer)[outChannel];
            outStride = 1;
        }
//        int
        for( i=0; i<framesPerBuffer; i++ )
        {
            tmpInput[i] = in[i];
        }

        //denoise here
        separate(tmpInput, tmpOutput, st);

        // copy the denoised sound to the output buffer
        for(int i = 0; i < framesPerBuffer; i++){
            out[i] = tmpOutput[i];
        }

//        inputIndex = inputIndex == 480 ? 0 : inputIndex;
//        printf("----------------------------------------\n");


        if (inChannel < (config->numInputChannels - 1)) inChannel++;
        else inDone = 1;
        if (outChannel < (config->numOutputChannels - 1)) outChannel++;
        else outDone = 1;
    }
    return 0;
}

/*******************************************************************/
int main(void);

int main(void) {
    PaError err = paNoError;
    WireConfig_t CONFIG;
    WireConfig_t *config = &CONFIG;
    int configIndex = 0;;

    err = Pa_Initialize();
    if (err != paNoError) goto error;

    printf("Please connect audio signal to input and listen for it on output!\n");
    printf("input format = %lu\n", INPUT_FORMAT);
    printf("output format = %lu\n", OUTPUT_FORMAT);
    printf("input device ID  = %d\n", INPUT_DEVICE);
    printf("output device ID = %d\n", OUTPUT_DEVICE);

    if (INPUT_FORMAT == OUTPUT_FORMAT) {
        gInOutScaler = 1.0;
    } else if ((INPUT_FORMAT == paInt16) && (OUTPUT_FORMAT == paFloat32)) {
        gInOutScaler = 1.0 / 32768.0;
    } else if ((INPUT_FORMAT == paFloat32) && (OUTPUT_FORMAT == paInt16)) {
        gInOutScaler = 32768.0;
    }

    for (config->isInputInterleaved = 0; config->isInputInterleaved < 2; config->isInputInterleaved++) {
        for (config->isOutputInterleaved = 0; config->isOutputInterleaved < 2; config->isOutputInterleaved++) {
            for (config->numInputChannels = 1; config->numInputChannels < 3; config->numInputChannels++) {
                for (config->numOutputChannels = 1; config->numOutputChannels < 3; config->numOutputChannels++) {
                    /* If framesPerCallback = 0, assertion fails in file pa_common/pa_process.c, line 1413: EX. */
                    for (config->framesPerCallback = 480;
                         config->framesPerCallback < 961; config->framesPerCallback += 480) {
                        printf("-----------------------------------------------\n");
                        printf("Configuration #%d\n", configIndex++);
                        err = TestConfiguration(config);
                        /* Give user a chance to bail out. */
                        if (err == 1) {
                            err = paNoError;
                            goto done;
                        } else if (err != paNoError) goto error;
                    }
                }
            }
        }
    }

    done:
    Pa_Terminate();
    printf("Full duplex sound test complete.\n");
    fflush(stdout);
    printf("Hit ENTER to quit.\n");
    fflush(stdout);
    getchar();
    return 0;

    error:
    Pa_Terminate();
    fprintf(stderr, "An error occurred while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
    printf("Hit ENTER to quit.\n");
    fflush(stdout);
    getchar();
    return -1;
}

static PaError TestConfiguration(WireConfig_t *config) {
    int c;
    PaError err = paNoError;
    PaStream *stream;
    PaStreamParameters inputParameters, outputParameters;

    printf("input %sinterleaved!\n", (config->isInputInterleaved ? " " : "NOT "));
    printf("output %sinterleaved!\n", (config->isOutputInterleaved ? " " : "NOT "));
    printf("input channels = %d\n", config->numInputChannels);
    printf("output channels = %d\n", config->numOutputChannels);
    printf("framesPerCallback = %d\n", config->framesPerCallback);

    inputParameters.device = INPUT_DEVICE;              /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default input device.\n");
        goto error;
    }
    inputParameters.channelCount = config->numInputChannels;
    inputParameters.sampleFormat = INPUT_FORMAT | (config->isInputInterleaved ? 0 : paNonInterleaved);
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = OUTPUT_DEVICE;            /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = config->numOutputChannels;
    outputParameters.sampleFormat = OUTPUT_FORMAT | (config->isOutputInterleaved ? 0 : paNonInterleaved);
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    config->numInputUnderflows = 0;
    config->numInputOverflows = 0;
    config->numOutputUnderflows = 0;
    config->numOutputOverflows = 0;
    config->numPrimingOutputs = 0;
    config->numCallbacks = 0;

    err = Pa_OpenStream(
            &stream,
            &inputParameters,
            &outputParameters,
            SAMPLE_RATE,
            config->framesPerCallback, /* frames per buffer */
            paClipOff, /* we won't output out of range samples so don't bother clipping them */
            wireCallback,
            config);
    if (err != paNoError) goto error;

    err = Pa_StartStream(stream);
    if (err != paNoError) goto error;

    printf("Now recording and playing. - Hit ENTER for next configuration, or 'q' to quit.\n");
    fflush(stdout);
    c = getchar();

    printf("Closing stream.\n");
    err = Pa_CloseStream(stream);
    if (err != paNoError) goto error;

#define CHECK_FLAG_COUNT(member) \
    if( config->member > 0 ) printf("FLAGS SET: " #member " = %d\n", config->member );
    CHECK_FLAG_COUNT(numInputUnderflows);
    CHECK_FLAG_COUNT(numInputOverflows);
    CHECK_FLAG_COUNT(numOutputUnderflows);
    CHECK_FLAG_COUNT(numOutputOverflows);
    CHECK_FLAG_COUNT(numPrimingOutputs);
    printf("number of callbacks = %d\n", config->numCallbacks);

    if (c == 'q') return 1;

    error:
    return err;
}
