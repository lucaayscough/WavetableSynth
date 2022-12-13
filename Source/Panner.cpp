#include "Headers.h"


//================================================//
// Pan class used to pan an audio signal.

Panner::Panner() {}

Panner::~Panner() {}


//================================================//
// Setter methods.

/**
    Sets the pan value and updates tha gain values.
    @param pan Pan value to be used.
 */

void Panner::setPan (float pan)
{
    m_Pan = pan;
    setGains();
}


//================================================//
// Getter methods.

float Panner::getPan()                                              { return m_Pan; }


//================================================//
// Pan class used to pan an audio signal.

/**
    Sets the new gain value for left and right channels.
 */

void Panner::setGains()
{
    m_LeftGain = 1.0;
    m_RightGain = 1.0;
    
    if (m_Pan < 0.0)
        m_RightGain += m_Pan;
    
    else if (m_Pan > 0.0)
        m_LeftGain -= m_Pan;
}


//================================================//
// DSP methods.

/**
    Adds panning to a given buffer.
    @param buffer Reference to an audio buffer which will be panned.
 */

void Panner::processBlock (juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() != 2)
        return;
    
    buffer.applyGain (0, 0, buffer.getNumSamples(), m_LeftGain);
    buffer.applyGain (1, 0, buffer.getNumSamples(), m_RightGain);
}

/**
    Adds panning to a given buffer.
    @param buffer Reference to an audio buffer which will be panned.
    @param panValues Reference to an array of pan values to be applied on a per sample basis.
 */

void Panner::processBlock (juce::AudioBuffer<float>& buffer, juce::Array<float>& panValues)
{
    if (buffer.getNumChannels() != 2)
        return;
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        setPan (panValues[sample]);
        
        buffer.applyGain (0, sample, 1, m_LeftGain);
        buffer.applyGain (1, sample, 1, m_RightGain);
    }
}
