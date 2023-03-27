#ifndef DFT_RESCALE_H
#define DFT_RESCALE_H

#include <valarray>
#include <complex>


typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

class DFT_rescale {
public:
    DFT_rescale(double sampleRate, double rescaleFactor);
    ~DFT_rescale();

    void process(float *samples, int numSamples);
    void fft(CArray& x);
    void ifft(CArray& x);
    void rescale(CArray& x, double factor);

private:
    double sampleRate_;
    double factor_;
    std::valarray<double> re_;
    std::valarray<double> im_;
};

#endif // DFT_RESCALE_H
