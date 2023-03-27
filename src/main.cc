#include "lowpass_filter.h"
#include "highpass_filter.h"
#include "dft_rescale.h"
#include "portaudio.h"

#include <iostream>
#include <cmath>
#include <vector>

#define SAMPLE_RATE (16000)
#define FRAMES_PER_BUFFER (512)

using namespace std;

int processAudio(const void *inputBuffer, void *outputBuffer,
                 unsigned long framesPerBuffer,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags, void *userData)
{

    if (!userData) 
    {
        cout << "here " << endl;
        return paAbort;
    }
    float *in = (float*)inputBuffer;
    float *out = (float*)outputBuffer;
    DFT_rescale *filter = (DFT_rescale*)userData;
    filter->process(in, framesPerBuffer);
    // Write input directly to output
    for (int i = 0; i < framesPerBuffer; i++) 
    {
        *out++ = *in++;
    }

    return paContinue;
}


int main()
{
    PaStream *stream;
    PaError err;
    
    DFT_rescale filter(SAMPLE_RATE, 0.6);

    err = Pa_Initialize();
    if (err != paNoError) {
        cout << "Error: failed to initialize PortAudio: " << Pa_GetErrorText(err) << endl;
        return 1;
    }
    
    err = Pa_OpenDefaultStream(&stream, 1, 1, paFloat32, SAMPLE_RATE,
                               FRAMES_PER_BUFFER, processAudio, &filter);
    if (err != paNoError) 
    {
        cout << "Error: failed to open audio stream: " << Pa_GetErrorText(err) << endl;
        Pa_Terminate();
        return 1;
    }
    
    err = Pa_StartStream(stream);
    if (err != paNoError) 
    {
        cout << "Error: failed to start audio stream: " << Pa_GetErrorText(err) << endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }
    
    cout << "Press ENTER to quit..." << endl;
    getchar();
    
    err = Pa_StopStream(stream);
    if (err != paNoError) 
    {
        cout << "Error: failed to stop audio stream: " << Pa_GetErrorText(err) << endl;
    }
    
    err = Pa_CloseStream(stream);
    if (err != paNoError) 
    {
        cout << "Error: failed to close audio stream: " << Pa_GetErrorText(err) << endl;
    }
    
    Pa_Terminate();
    return 0;
}
