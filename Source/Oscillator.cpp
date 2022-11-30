#include "Headers.h"


//================================================//
// Base oscillator class.

Oscillator::Oscillator() {}
Oscillator::~Oscillator() {}

void Oscillator::setFrequency (float frequency)
{
    m_frequency = frequency;
    updatePhaseDelta();
}

void Oscillator::setSampleRate (float sampleRate)                   { m_sampleRate = sampleRate; }
void Oscillator::setBlockSize (int blockSize)                       { m_blockSize = blockSize; }
void Oscillator::setPhase (float phase)                             { m_phase = phase; }
void Oscillator::setPhaseDelta (float phaseDelta)                   { m_phaseDelta = phaseDelta; }

float Oscillator::getFrequency()                                    { return m_frequency; }
float Oscillator::getSampleRate()                                   { return m_sampleRate; }
int Oscillator::getBlockSize()                                      { return m_blockSize; }
float Oscillator::getPhase()                                        { return m_phase; }
float Oscillator::getPhaseDelta()                                   { return m_phaseDelta; }
juce::AudioBuffer<float>& Oscillator::getBlock()                    { return m_buffer; }

void Oscillator::updatePhase()
{
    m_phase += m_phaseDelta;
    
    if (m_phase > 1.0f)
        m_phase -= 1.0f;
}

void Oscillator::updatePhaseDelta()
{
    jassert (m_frequency > 0.f && m_sampleRate > 0.f);
    
    m_phaseDelta = m_frequency / m_sampleRate;
}

void Oscillator::prepareToPlay (float sampleRate)
{
    setSampleRate (sampleRate);
}

void Oscillator::prepareToPlay (float sampleRate, int blockSize)
{
    setSampleRate (sampleRate);
    setBlockSize (blockSize);
    m_buffer.setSize(1, blockSize);
}

float Oscillator::output (float phase)
{
    return phase;
}

float Oscillator::processSample()
{
    float sample = output (m_phase);
    updatePhase();
    
    return sample;
}

juce::AudioBuffer<float>& Oscillator::processBlock()
{
    m_buffer.clear();
    auto* bufferData = m_buffer.getWritePointer (0);
    
    for (int sample = 0; sample < m_blockSize; ++sample)
    {
        bufferData[sample] = output (m_phase);
        updatePhase();
    }
    
    return m_buffer;
}

void Oscillator::processBlock(juce::AudioBuffer<float>& block)
{
    for (int sample = 0; sample < block.getNumSamples(); ++sample)
    {
        for (int channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getWritePointer (channel);
            channelData[sample] += output (m_phase);
        }
        
        updatePhase();
    }
}

void Oscillator::processBlock(juce::AudioBuffer<float>& block, int startSample, int numSamples)
{
    for (int sample = startSample; sample < startSample + numSamples; ++sample)
    {
        for (int channel = 0; channel < block.getNumChannels(); ++channel)
        {
            auto* channelData = block.getWritePointer (channel);
            channelData[sample] += output (m_phase);
        }
        
        updatePhase();
    }
}


//================================================//
// Sine wave oscillator.

SineOscillator::SineOscillator() {}

float SineOscillator::output (float phase)
{
    return std::sin(2.0f * phase * M_PI);
}


//================================================//
// Square wave oscillator.

SquareOscillator::SquareOscillator() {}

/**
    Outputs the sample value for the current phase based on square wave algorithm.
    @param phase Phase to use.
 */

float SquareOscillator::output (float phase)
{
    if (phase < 0.5f)
        return 1.0f;
    
    else
        return -1.0f;
}


//================================================//
// Pulse wave oscillator.

PulseOscillator::PulseOscillator() {}

void PulseOscillator::setPulseWidth (float pulseWidth)              { m_PulseWidth = pulseWidth; }
float PulseOscillator::getPulseWidth()                              { return m_PulseWidth; }

float PulseOscillator::output (float phase)
{
    if (phase < m_PulseWidth)
        return 1.0f;
    
    else
        return -1.0f;
}


//================================================//
// Triangle wave oscillator.

TriangleOscillator::TriangleOscillator() {}

float TriangleOscillator::output (float phase)
{
    // Algorithm used to compute triangle wave in range [-1,1] --> https://wikimedia.org/api/rest_v1/media/math/render/svg/bc9fd743afd5943b7f83248e59d55d97119257b9
    return 2.0f * std::fabs(2.0f * (phase - std::floor(phase + 0.5f))) - 1.0f;
}


//================================================//
// Sawtooth wave oscillator.

SawtoothOscillator::SawtoothOscillator() {}

float SawtoothOscillator::output (float phase)
{
    // Algorithm used to compute sawtooth wave in range [-1,1] --> https://wikimedia.org/api/rest_v1/media/math/render/svg/0f07cb8c8f5850b17ad8c3415800046cd1f38967
    return 2.0f * (phase - std::floor(0.5f + phase));
}
