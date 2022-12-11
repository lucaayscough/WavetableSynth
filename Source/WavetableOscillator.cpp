#include "Headers.h"


WavetableOscillator::WavetableOscillator() {}
WavetableOscillator::~WavetableOscillator() {}

float WavetableOscillator::getSampleRate()                              { return m_sampleRate; }
float WavetableOscillator::getFrequency()                               { return m_frequency; }

void WavetableOscillator::setSampleRate (float sampleRate)              { m_sampleRate = sampleRate; }
void WavetableOscillator::setFrequency (float frequency)                { m_frequency = frequency; }

float WavetableOscillator::processSample ()
{
    return 0.f;
}
