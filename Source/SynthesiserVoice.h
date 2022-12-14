#pragma once


class SynthesiserSound : public juce::SynthesiserSound
{
public:
    SynthesiserSound();
    ~SynthesiserSound();
    
    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthesiserSound)
};


class SynthesiserVoice : public juce::SynthesiserVoice
{
public:
    
    //================================================//
    /// Constructors.
    
    SynthesiserVoice();
    
    
    //================================================//
    /// Destructors.
    
    ~SynthesiserVoice();
    
    
    //================================================//
    /// Setter methods.
    
    /**
        Sets the number of active voices.
        @param numActiveVoices The number of active voices to use.
     */
    
    void setNumActiveVoices (int numActiveVoices);
    
    /**
        Sets the detune amount.
        @param detune The detune amount to use.
     */
    
    void setDetune (float detune);
    
    /**
        Converts a pitch bend midi message into a scaled value in range [-1,1]
        and stores the computed value.
        @param pitchWheelValue Pitch wheel value to use.
     */
    
    void setPitchBend (int pitchWheelValue);
    
    /**
        Sets the envelope for each voice,
        @param attack The attack stage duration to be used.
        @param decay The decay stage duration to be used.
        @param sustain The sustain stage amount to be used.
        @param release The release stage duration to be used.
     */
    
    void setEnvelope (float attack, float decay, float sustain, float release);
    
    /**
        Sets the number of active oscillators for each voice.
        @param numActiveVoices The number of voices to be used.
     */
    
    void setWavetablePosition (float position);
    
    
    //================================================//
    /// Modulation methods.
    
    /**
        Modulates the frequency of the voices oscillators.
        @param modulationAmount The amount of modulation to apply.
     */
    
    void modulateFrequency (float modulationAmount);
    
    /**
        Return a float representing the detune amount to be applied to a given voice.
        Each voice will have a slightly different amount of detune.
        @param voice The voice index to be used to compute the detune amount.
     */
    
    float getDetuneAmount (int voice);
    
    /**
        Return a float representing the amount of frequency modulation to apply.
     */
    
    float getPitchBendModulation();
    
    
    //================================================//
    /// Inherited methods.
    
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void setCurrentPlaybackSampleRate (double newRate) override;
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
private:
    bool m_playing = false;                                         // The playing state of the oscillator.
    
    juce::OwnedArray<WavetableOscillator> m_voices;                 // Array holding pointers to all the oscillators.
    int m_numActiveVoices;                                          // Number of currently active voices.
    
    juce::ADSR m_adsr;                                              // Envelope to use on the oscillator outputs.
    
    float m_detune;                                                 // Stores the current detune amount.
    float m_velocity;                                               // Stores the last velocity midi value received.
    float m_pitchBend;                                              // Stores the pitch bend amount.
    
    float m_frequency;                                              // The frequency of the voice.
    
    juce::OwnedArray<juce::IIRFilter> m_filters;                    // Array of pointers to filters.
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesiserVoice)
};
