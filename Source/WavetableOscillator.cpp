#include "Headers.h"


WavetableOscillator::WavetableOscillator()
{
    m_wavetable.ensureStorageAllocated (m_wavetableLength);
    
    for (int i = 0; i < m_wavetableLength; ++i)
    {
        m_wavetable.add (std::sinf(2.f * M_PI * static_cast<float> (i) / m_wavetableLength));
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

void WavetableOscillator::updateIndexIncrement()
{
    m_indexIncrement = m_frequency * static_cast<float>(m_wavetableLength) / static_cast<float>(m_sampleRate);
}

float WavetableOscillator::interpolateLinearly()
{
    const auto truncatedIndex = static_cast<int> (m_index);
    const auto nextIndex = static_cast<int> (std::ceil (m_index)) % m_wavetableLength;
    const auto nextIndexWeight = m_index - static_cast<float> (truncatedIndex);
    return m_wavetable[nextIndex] * nextIndexWeight + (1.f - nextIndexWeight) * m_wavetable[truncatedIndex];
}

void WavetableOscillator::prepareToPlay (float sampleRate)
{
    setSampleRate (sampleRate);
}

float WavetableOscillator::processSample()
{
    m_index = std::fmod (m_index, static_cast<float> (m_wavetableLength));
    const auto sample = interpolateLinearly();
    m_index += m_indexIncrement;
    return sample;
}
