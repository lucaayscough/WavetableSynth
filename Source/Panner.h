#pragma once


//================================================//
/// Pan class used to pan an audio signal.

class Panner
{
public:
    Panner();
    ~Panner();
    
    // Setter methods.
    void setPan (float pan);
    
    // Getter methods.
    float getPan();
    
    // Helper methods.
    void setGains();
    
    // DSP methods.
    void processBlock (juce::AudioBuffer<float>& buffer);
    void processBlock (juce::AudioBuffer<float>& buffer, juce::Array<float>& panValues);
    
    
private:
    float m_Pan = 0.0;                          // Pan value in range [-1, 1].
    float m_LeftGain = 1.0;                     // Gain for left channel.
    float m_RightGain = 1.0;                    // Gain for right channel.
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Panner)
};
