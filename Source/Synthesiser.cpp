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

void Synthesiser::setNumActiveVoices (float numActiveVoices)
{
    for (int voiceIndex = 0; voiceIndex < getNumVoices(); ++voiceIndex)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*> (getVoice (voiceIndex));
        v->setNumActiveVoices (numActiveVoices);
    }
}

void Synthesiser::setDetune (float detune)
{
    for (int voiceIndex = 0; voiceIndex < getNumVoices(); ++voiceIndex)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*> (getVoice (voiceIndex));
        v->setDetune (detune);
    }
}

void Synthesiser::setEnvelope (float attack, float decay, float sustain, float release)
{
    for (int voiceIndex = 0; voiceIndex < getNumVoices(); ++voiceIndex)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*> (getVoice (voiceIndex));
        v->setEnvelope(attack, decay, sustain, release);
    }
}
