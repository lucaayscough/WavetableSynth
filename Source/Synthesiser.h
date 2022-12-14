#pragma once


/// Base synthesiser class inheriting from the juce synthesiser class.

class Synthesiser : public juce::Synthesiser
{
public:
    Synthesiser();
    ~Synthesiser();
    
    void setWavetablePosition (float position);
    void setNumActiveVoices (float numActiveVoices);
    void setDetune (float detune);
    void setEnvelope (float attack, float decay, float sustain, float release);
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesiser)
};
