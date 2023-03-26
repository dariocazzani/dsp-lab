#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

class LowpassFilter {
public:
    LowpassFilter(double sampleRate, double cutoffFrequency, double resonance = 1.0);
    ~LowpassFilter();

    void process(float *samples, int numSamples);

private:
    double sampleRate_;
    double cutoffFrequency_;
    double resonance_;
    double a0_, a1_, a2_, b1_, b2_;
    double x1_, x2_, y1_, y2_;
};

#endif // LOWPASS_FILTER_H
