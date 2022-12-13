#pragma once


class Synthesiser : public juce::Synthesiser
{
public:
    Synthesiser();
    ~Synthesiser();
    
    void setWavetablePosition (float position);
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesiser)
};
