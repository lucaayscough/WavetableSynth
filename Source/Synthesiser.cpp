#include "Headers.h"


Synthesiser::Synthesiser() {}

Synthesiser::~Synthesiser() {}

void Synthesiser::setWavetable()
{
    m_formatManager.registerBasicFormats();
    
    auto* reader = m_formatManager.createReaderFor (std::make_unique<juce::MemoryInputStream> (BinaryData::ESW_Analog__303_Saw_wav, BinaryData::ESW_Analog__303_Saw_wavSize, false));
    
    for (int voiceIndex = 0; voiceIndex < getNumVoices(); ++voiceIndex)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*> (getVoice (voiceIndex));
        v->setWavetable (reader);
    }
}
