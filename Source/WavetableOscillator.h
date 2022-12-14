#pragma once


//================================================//
/// Wavetable oscillator class which generates audio signals based
/// on a pre-computed wavetable.

class WavetableOscillator
{
public:
    
    //================================================//
    /// Constructors.
    
    WavetableOscillator();
    
    
    //================================================//
    /// Destructor.
    
    ~WavetableOscillator();
    
    
    //================================================//
    /// Getter methods.
    
    /**
        Returns a float representing the sample rate.
     */
    
    float getSampleRate();
    
    /**
        Returns a float representing the frequency.
     */
    
    float getFrequency();
    
    
    //================================================//
    /// Setter methods.
    
    /**
        Sets the sample rate.
        @param sampleRate Sample rate to use.
     */
    
    void setSampleRate (float sampleRate);
    
    /**
        Sets the frequency.
        @param frequency Frequency to use.
     */
    
    void setFrequency (float frequency);
    
    /**
        Sets the wavetable index.
        @param index Index to use.
     */
    
    void setIndex (float index);
    
    /**
        Sets the wavetable frame.
        @param frame Frame to use.
     */
    
    void setFrame (float frame);
    
    
    //================================================//
    /// DSP methods.
    
    /**
        Updates the index increment value.
     */
    
    void updateIndexIncrement();
    
    /**
        Returns a float value representing the interpolated value
        of two samples at a specific index in the wavetable.
     */
    
    float interpolateLinearly();
    
    /**
        Prepares the oscillator for playback.
        @param sampleRate Sample rate to use.
     */
    
    void prepareToPlay (float sampleRate);
    
    /**
        Returns a float value representing a single sample in the wave cycle.
     */
    
    float processSample();

private:
    float m_sampleRate = 0.f;                                           // The oscillator sample rate.
    float m_frequency = 0.f;                                            // The oscillator frequency.
    
    static const int m_wavetableLength = 2048;                          // The number of samples representing a single cycle of the pre-computed wavetable.
    static const int m_numFrames = 256;                                 // The number of frames representing each a different waverform.
    
    float m_wavetable[m_wavetableLength][m_numFrames];                  // 2-Dimensional array storing the pre-computed wavetable.
    
    float m_index = 0.f;                                                // The current wavetable index.
    float m_indexIncrement = 0.f;                                       // Value by which the index of the wavetable will be incremented.
    
    int m_frame = 0;                                                    // The current frame being used.
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableOscillator)
};
