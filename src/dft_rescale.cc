#include "dft_rescale.h"
#include <cmath>
#include <iostream>

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

void DFT_rescale::fft(CArray &x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
}

void DFT_rescale::ifft(CArray& x)
{
    // conjugate the complex numbers
    x = x.apply(std::conj);

    // forward fft
    fft( x );

    // conjugate the complex numbers again
    x = x.apply(std::conj);

    // scale the numbers
    x /= x.size();
}

void DFT_rescale::rescale(CArray& x, double factor)
{
    const bool parity = (x.size() % 2 == 0);
    const int N = (parity ? x.size() / 2 + 1 : (x.size() + 1) / 2);

    // Create rescaled spectrum
    CArray Y(N);
    Y = Complex(0.0, 0.0);
    for (size_t n = 0; n < N; ++n)
    {
        size_t ix = static_cast<size_t>(std::round(n * factor));
        Y[ix] += x[n];
        if (ix >= N) break;
    }

    CArray Y_slice(x.size()-N);
    if (parity)
    {
      Y_slice = Y[std::slice(N - 2, x.size()-N, -1)];
    }
    else
    {
      Y_slice = Y[std::slice(N - 1, x.size()-N, -1)];
    }

    Y_slice = Y_slice.cshift(-1);
    
    CArray Y_conj = Y_slice.apply(std::conj);
    CArray Y_out = std::valarray<Complex>(Y.size() + Y_conj.size());
    
    std::copy(std::begin(Y), std::end(Y), std::begin(Y_out));
    std::copy(std::begin(Y_conj), std::end(Y_conj), std::begin(Y_out)+Y.size());

    std::cout << "Y out size: " << Y_out.size() << std::endl;
    
    // Copy back the rescaled spectrum to x
    x = Y_out;
}
