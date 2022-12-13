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
    SynthesiserVoice();
    ~SynthesiserVoice();
    
    void setPitchBend (int pitchWheelValue);
    void setEnvelope (float attack, float decay, float sustain, float release);
    void setWavetablePosition (float position);
    
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    
    void setCurrentPlaybackSampleRate (double newRate) override;

    void modulateFrequency (float modulationAmount);
    void pitchBendModulation();
    
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
private:
    bool m_playing = false;
    
    WavetableOscillator m_voice;
    
    juce::ADSR m_adsr;
    
    float m_velocity;
    float m_pitchBend;
    
    float m_frequency;
    
    juce::OwnedArray<juce::IIRFilter> m_filters;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesiserVoice)
};
