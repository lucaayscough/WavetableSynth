#pragma once


class Synthesiser : public juce::Synthesiser
{
public:
    Synthesiser();
    ~Synthesiser();
    
    void setWavetable();
    
private:
    juce::AudioFormatManager m_formatManager;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesiser)
};
