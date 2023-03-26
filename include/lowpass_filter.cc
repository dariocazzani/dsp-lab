#include "lowpass_filter.h"
#include <cmath>

LowpassFilter::LowpassFilter(double sampleRate, double cutoffFrequency, double resonance)
    : sampleRate_(sampleRate), cutoffFrequency_(cutoffFrequency), resonance_(resonance),
      a0_(0), a1_(0), a2_(0), b1_(0), b2_(0), x1_(0), x2_(0), y1_(0), y2_(0)
{
    double omega = 2.0 * M_PI * cutoffFrequency / sampleRate;
    double cosw = std::cos(omega);
    double alpha = std::sin(omega) / (2.0 * resonance_);

    double b0 = (1.0 - cosw) / 2.0;
    double b1 = 1.0 - cosw;
    double b2 = (1.0 - cosw) / 2.0;
    double a0 = 1.0 + alpha;
    double a1 = -2.0 * cosw;
    double a2 = 1.0 - alpha;

    double coeff = 1.0 / a0;

    a0_ = a0 * coeff;
    a1_ = a1 * coeff;
    a2_ = a2 * coeff;
    b1_ = b1 * coeff;
    b2_ = b2 * coeff;
}

LowpassFilter::~LowpassFilter()
{
}

void LowpassFilter::process(float *samples, int numSamples)
{
    double x0, y0;
    for (int i = 0; i < numSamples; i++) {
        x0 = samples[i];
        y0 = a0_ * x0 + a1_ * x1_ + a2_ * x2_ - b1_ * y1_ - b2_ * y2_;
        x2_ = x1_;
        x1_ = x0;
        y2_ = y1_;
        y1_ = y0;
        samples[i] = (float)y0;
    }
}
