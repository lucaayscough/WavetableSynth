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
    void setIndex (float index);
    void setWavetable (juce::AudioFormatReader* audioFormatReader, float position);
    
    void updateIndexIncrement();
    
    float interpolateLinearly();
    
    void prepareToPlay (float sampleRate);
    
    float processSample();

private:
    float m_sampleRate = 0.f;
    float m_frequency = 0.f;
    
    int m_wavetableLength = 256;
    float m_index = 0.f;
    float m_indexIncrement = 0.f;
    juce::AudioBuffer<float> m_wavetable;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableOscillator)
};
