#include "dft_rescale.h"
#include <cmath>

const double PI = 3.141592653589793238460;


DFT_rescale::DFT_rescale(double sampleRate, double factor) : sampleRate_(sampleRate), factor_(factor)
{
}

DFT_rescale::~DFT_rescale()
{
}

void DFT_rescale::process(float *samples, int numSamples)
{
    // Convert samples to complex numbers
    CArray x(numSamples);
    for (size_t i=0; i<numSamples; ++i)
    {
      x[i] = Complex(samples[i], 0.0);
    }
    

    // Forward FFT
    fft(x);

    // Rescale the spectrum
    rescale(x, factor_);

    // Inverse FFT
    ifft(x);

    // Copy the real part of the result back to samples
    for (int i = 0; i < numSamples; i++) {
        samples[i] = (float)x[i].real();
    }
}

void DFT_rescale::fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;

    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];

    // conquer
    fft(even);
    fft(odd);

    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
}

void DFT_rescale::ifft(CArray& x)
{
    // conjugate the complex numbers
    x = x.apply(std::conj);

    // forward fft
    fft(x);

    // conjugate the complex numbers again
    x = x.apply(std::conj);

    // scale the numbers
    x /= x.size();
}

void DFT_rescale::rescale(CArray& x, double factor)
{
    const size_t N = x.size();
    const size_t kMax = static_cast<size_t>(std::ceil(N / factor));

    // Create rescaled spectrum
    CArray y(N);
    for (size_t k = 0; k < kMax; ++k)
    {
        size_t i = static_cast<size_t>(std::round(k * factor));
        y[k] = x[i];
    }

    // Zero out the rest of the spectrum
    for (size_t k = kMax; k < N; ++k)
    {
        y[k] = Complex(0, 0);
    }

    // Copy back the rescaled spectrum to x
    x = y;
}
