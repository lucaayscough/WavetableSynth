#pragma once


class WavetableOscillator
{
public:
    WavetableOscillator();
    ~WavetableOscillator();
    
    float getSampleRate();
    float getFrequency();
    
    void setSampleRate (float sampleRate);
    void setFrequency (float frequency);
    
    float processSample();

private:
    float m_sampleRate;
    float m_frequency;
    juce::Array<float> m_wavetable;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableOscillator)
};
