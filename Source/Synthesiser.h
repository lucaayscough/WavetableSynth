#pragma once


//================================================//
/// Base synthesiser which inherits from the juce synthesiser class.

class Synthesiser : public juce::Synthesiser
{
public:
    
    //================================================//
    /// Constructors.
    
    Synthesiser();
    
    
    //================================================//
    /// Destructor.
    
    ~Synthesiser();
    
    
    //================================================//
    /// Setter methods.
    
    /**
        Sets the wavetable position for all voices.
        @param position The wavetable position to be used.
     */
    
    void setWavetablePosition (float position);
    
    /**
        Sets the number of active oscillators for each voice.
        @param numActiveVoices The number of voices to be used.
     */
    
    void setNumActiveVoices (float numActiveVoices);
    
    /**
        Sets the detune amount for the oscillators in each voice.
        @param detune The detune amount.
     */
    
    void setDetune (float detune);
    
    /**
        Sets the envelope for each voice,
        @param attack The attack stage duration to be used.
        @param decay The decay stage duration to be used.
        @param sustain The sustain stage amount to be used.
        @param release The release stage duration to be used.
     */
    
    void setEnvelope (float attack, float decay, float sustain, float release);
    
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Synthesiser)
};
