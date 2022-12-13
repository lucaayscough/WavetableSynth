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
    void setFrame (float frame);
    
    void updateIndexIncrement();
    
    float interpolateLinearly();
    
    void prepareToPlay (float sampleRate);
    
    float processSample();

private:
    float m_sampleRate = 0.f;
    float m_frequency = 0.f;
    
    static const int m_wavetableLength = 2048;
    static const int m_numFrames = 256;
    
    float m_wavetable[m_wavetableLength][m_numFrames];
    
    float m_index = 0.f;
    float m_indexIncrement = 0.f;
    
    int m_frame = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableOscillator)
};
