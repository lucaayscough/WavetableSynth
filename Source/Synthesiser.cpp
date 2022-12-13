#include "Headers.h"


Synthesiser::Synthesiser() {}

Synthesiser::~Synthesiser() {}

void Synthesiser::setWavetablePosition (float position)
{
    for (int voiceIndex = 0; voiceIndex < getNumVoices(); ++voiceIndex)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*> (getVoice (voiceIndex));
        v->setWavetablePosition (position);
    }
}
