#pragma once


//================================================//
/// Base oscillator class which outputs a phasor wave.

class Oscillator
{
public:
    Oscillator();
    virtual ~Oscillator();
    
    
    //================================================//
    // Setter methods.
    
    void setFrequency (float frequency);
    void setSampleRate (float sampleRate);
    void setBlockSize (int blockSize);
    void setPhase (float phase);
    void setPhaseDelta (float phaseDelta);
    
    
    //================================================//
    // Getter methods.
    
    float getFrequency();
    float getSampleRate();
    int getBlockSize();
    float getPhase();
    float getPhaseDelta();
    juce::AudioBuffer<float>& getBlock();
    
    
    //================================================//
    // Phase methods.
    
    /**
        Updates the phase of the oscillator's cycle by adding a phase delta value.
     */
    
    void updatePhase();
    
    /**
        Updates the phase delta of the oscllator based on the current frequency
        and sample rate.
     */

    void updatePhaseDelta();
    
    
    //================================================//
    // Init methods.
    
    /**
        Prepares the oscillator for playback.
        @param sampleRate Sample rate to use.
     */
    
    void prepareToPlay (float sampleRate);
    
    /**
        Prepares the oscillator for playback.
        @param sampleRate Sample rate to use.
        @param blockSize Block size to use.
     */
    
    void prepareToPlay (float sampleRate, int blockSize);
    
    
    //================================================//
    // DSP methods.

    /**
        Virtual method that returns the phase to be outputted.
        @param phase Phase to use.
     */
    
    virtual float output (float phase);
    
    /**
        Processes a single sample, updates the phase of the oscillator and returns the sample.
     */
    
    float processSample();
    
    /**
        Processes a block of samples, updates the phase of the oscillator and returns the block.
     */

    juce::AudioBuffer<float>& processBlock();
    
    /**
        Processes a block of samples and updates the phase of the oscillator.
        @param block Buffer to be filled.
     */

    void processBlock(juce::AudioBuffer<float>& block);
    
    /**
        Processes a block of samples and updates the phase of the oscillator.
        @param block Buffer to be filled.
        @param startSample Index of the buffer to start filling from.
        @param numSamples Number of samples to fill the buffer with.
     */

    void processBlock(juce::AudioBuffer<float>& block, int startSample, int numSamples);
    
private:
    float m_frequency = 0;
    float m_sampleRate = 0;
    float m_blockSize = 0;
    float m_phase = 0;
    float m_phaseDelta = 0;
    juce::AudioBuffer<float> m_buffer;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};


//================================================//
/// Square wave oscillator class.

class SineOscillator : public Oscillator
{
public:
    SineOscillator();
    
    
    //================================================//
    // DSP methods.
    
    /**
        Outputs the sample value for the current phase based on sine wave algorithm.
        @param phase Phase to use.
     */
    
    float output (float phase) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineOscillator)
};


//================================================//
/// Square wave oscillator class.

class SquareOscillator : public Oscillator
{
public:
    SquareOscillator();
    
    // DSP methods.
    float output (float phase) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SquareOscillator)
};


//================================================//
/// Pulse wave oscillator class.

class PulseOscillator : public Oscillator
{
public:
    PulseOscillator();
    
    
    //================================================//
    // Setter methods.
    
    void setPulseWidth (float pulseWidth);
    
    
    //================================================//
    // Getter methods.
    
    float getPulseWidth();
    

    //================================================//
    // DSP methods.

    /**
        Outputs the sample value for the current phase based on pulse wave algorithm.
        @param phase Phase to use.
     */

    float output (float phase) override;
    
private:
    float m_PulseWidth = 0.5f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PulseOscillator)
};


//================================================//
/// Triangle wave oscillator class.

class TriangleOscillator : public Oscillator
{
public:
    TriangleOscillator();
    
    
    //================================================//
    // DSP methods.

    /**
        Outputs the sample value for the current phase based on triangle wave algorithm.
        @param phase Phase to use.
     */
    
    float output (float phase) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriangleOscillator)
};


//================================================//
/// Sawtooth wave oscillator class.

class SawtoothOscillator : public Oscillator
{
public:
    SawtoothOscillator();

    
    //================================================//
    // DSP methods.

    /**
        Outputs the sample value for the current phase based on sawtooth wave algorithm.
        @param phase Phase to use.
     */

    float output (float phase) override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SawtoothOscillator)
};
