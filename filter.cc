#include <iostream>
#include <cmath>
#include "portaudio.h"

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (16)

using namespace std;

int processAudio(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags, void *userData)
{
    float *in = (float*)inputBuffer;
    float *out = (float*)outputBuffer;
    
    // Write input directly to output
    for (int i = 0; i < framesPerBuffer; i++) {
        *out++ = *in++;
    }
    
    return paContinue;
}

int main()
{
    PaStream *stream;
    PaError err;
    
    err = Pa_Initialize();
    if (err != paNoError) {
        cout << "Error: failed to initialize PortAudio: " << Pa_GetErrorText(err) << endl;
        return 1;
    }
    
    err = Pa_OpenDefaultStream(&stream, 1, 1, paFloat32, SAMPLE_RATE,
                               FRAMES_PER_BUFFER, processAudio, NULL);
    if (err != paNoError) {
        cout << "Error: failed to open audio stream: " << Pa_GetErrorText(err) << endl;
        Pa_Terminate();
        return 1;
    }
    
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        cout << "Error: failed to start audio stream: " << Pa_GetErrorText(err) << endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }
    
    cout << "Press ENTER to quit..." << endl;
    getchar();
    
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        cout << "Error: failed to stop audio stream: " << Pa_GetErrorText(err) << endl;
    }
    
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        cout << "Error: failed to close audio stream: " << Pa_GetErrorText(err) << endl;
    }
    
    Pa_Terminate();
    return 0;
}
