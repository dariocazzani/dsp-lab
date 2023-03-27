#ifndef HIGHPASS_FILTER_H
#define HIGHPASS_FILTER_H

class HighpassFilter {
public:
    HighpassFilter(double sampleRate, double cutoffFrequency, double resonance = 1.0);
    ~HighpassFilter();

    void process(float *samples, int numSamples);

private:
    double sampleRate_;
    double cutoffFrequency_;
    double resonance_;
    double a0_, a1_, a2_, b1_, b2_;
    double x1_, x2_, y1_, y2_;
};

#endif // HIGHPASS_FILTER_H
