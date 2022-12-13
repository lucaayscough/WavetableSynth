#include "Headers.h"


WavetableOscillator::WavetableOscillator()
{
    // TODO:
    // Tidy this up.
    
    juce::Array<float> sine;
    juce::Array<float> triangle;
    juce::Array<float> sawtooth;
    juce::Array<float> square;
    
    
    // Generate sine, triangle, sawtooth and square waves.
    for (int i = 0; i < m_wavetableLength; ++i)
    {
        float phase = static_cast<float> (i) / m_wavetableLength;
        
        sine.add (std::sinf (2.f * M_PI * phase));
        triangle.add (2.0f * std::fabs (2.0f * (phase - std::floor (phase + 0.5f))) - 1.0f);
        sawtooth.add (2.0f * (phase - std::floor (0.5f + phase)));
        
        if (phase < 0.5f)
        {
            square.add (1.f);
        }
        
        else
        {
            square.add (-1.f);
        }
    }
    
    for (int frame = 0; frame < m_numFrames; ++frame)
    {
        float weight = static_cast<float> (frame % 86) / static_cast<float> (86);
        DBG (weight);
        
        for (int sample = 0; sample < m_wavetableLength; ++sample)
        {
            if (frame < 86)
            {
                m_wavetable[sample][frame] = sine[sample] + weight * (triangle[sample] - sine[sample]);
            }
            
            else if (frame < 172)
            {
                m_wavetable[sample][frame] = triangle[sample] + weight * (sawtooth[sample] - triangle[sample]);
            }
            
            else
            {
                m_wavetable[sample][frame] = sawtooth[sample] + weight * (square[sample] - sawtooth[sample]);
            }
        }
    }
}

WavetableOscillator::~WavetableOscillator() {}

float WavetableOscillator::getSampleRate()                              { return m_sampleRate; }
float WavetableOscillator::getFrequency()                               { return m_frequency; }

void WavetableOscillator::setSampleRate (float sampleRate)              { m_sampleRate = sampleRate; }

void WavetableOscillator::setFrequency (float frequency)
{
    m_frequency = frequency;
    updateIndexIncrement();
}

void WavetableOscillator::setIndex (float index)                        { m_index = index; }
void WavetableOscillator::setFrame (float frame)                        { m_frame = frame; }

void WavetableOscillator::updateIndexIncrement()
{
    m_indexIncrement = m_frequency * static_cast<float> (m_wavetableLength) / static_cast<float> (m_sampleRate);
}

float WavetableOscillator::interpolateLinearly()
{
    const auto truncatedIndex = static_cast<int> (m_index);
    const auto nextIndex = static_cast<int> (std::ceil (m_index)) % m_wavetableLength;
    const auto nextIndexWeight = m_index - static_cast<float> (truncatedIndex);
    return m_wavetable[nextIndex][m_frame] * nextIndexWeight + (1.f - nextIndexWeight) * m_wavetable[truncatedIndex][m_frame];
}

void WavetableOscillator::prepareToPlay (float sampleRate)
{
    setSampleRate (sampleRate);
}

float WavetableOscillator::processSample()
{
    m_index = std::fmod (m_index, static_cast<float> (m_wavetableLength));
    //auto sample = m_wavetable[static_cast<int> (m_index)][static_cast<int> (m_frame)];
    const auto sample = interpolateLinearly();
    m_index += m_indexIncrement;
    return sample;
}
