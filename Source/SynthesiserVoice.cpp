#include "Headers.h"


SynthesiserSound::SynthesiserSound() {}
SynthesiserSound::~SynthesiserSound() {}

bool SynthesiserSound::appliesToNote (int midiNoteNumber) { return true; }
bool SynthesiserSound::appliesToChannel (int midiChannel) { return true; }


SynthesiserVoice::SynthesiserVoice()
{
    // Oscillators.
    for (int voice = 0; voice < m_numVoices; ++voice)
    {
        m_voices.add (new WavetableOscillator);
    }
    
    // ADSR envelope.
    juce::ADSR::Parameters parameters;
    
    parameters.attack = 0.1f;
    parameters.decay = 1.f;
    parameters.sustain = 0.5f;
    parameters.release = 0.1f;

    m_adsr.setParameters (parameters);
}

SynthesiserVoice::~SynthesiserVoice() {}

void SynthesiserVoice::setPitchBend (int pitchWheelValue)
{
    if (pitchWheelValue > 8192)
    {
        m_pitchBend = float(pitchWheelValue - 8192) / (16383 - 8192);
    }
    
    else
    {
        m_pitchBend = float(8192 - pitchWheelValue) / -8192;
    }
}

void SynthesiserVoice::setEnvelope (float attack, float decay, float sustain, float release)
{
    auto parameters = m_adsr.getParameters();
    
    parameters.attack = attack;
    parameters.decay = decay;
    parameters.sustain = sustain;
    parameters.release = release;
    
    m_adsr.setParameters (parameters);
}

void SynthesiserVoice::setWavetablePosition (float position)
{
    for (int voice = 0; voice < m_numActiveVoices; ++voice)
    {
        m_voices[voice]->setFrame (position);
    }
}

bool SynthesiserVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<SynthesiserSound*> (sound) != nullptr;
}

void SynthesiserVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    if (!m_playing)
    {
        m_playing = true;
        m_frequency = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        
        for (int voice = 0; voice < m_numActiveVoices; ++voice)
        {
            m_voices[voice]->setIndex (0.f);
            m_voices[voice]->setFrequency (m_frequency * getDetuneAmount (voice));
        }
        
        m_adsr.reset();
        m_adsr.noteOn();
        
        m_velocity = velocity;
        setPitchBend (currentPitchWheelPosition);
    }
}

void SynthesiserVoice::stopNote (float velocity, bool allowTailOff)
{
    m_adsr.noteOff();
}

void SynthesiserVoice::pitchWheelMoved (int newPitchWheelValue)
{
    setPitchBend (newPitchWheelValue);
}

void SynthesiserVoice::controllerMoved (int controllerNumber, int newControllerValue) {}

void SynthesiserVoice::setCurrentPlaybackSampleRate (double newRate)
{
    if (newRate != 0)
    {
        for (int voice = 0; voice < m_numActiveVoices; ++voice)
        {
            m_voices[voice]->setSampleRate (newRate);
        }
    
        m_adsr.setSampleRate (newRate);
        
        m_filters.clear();
        
        for (int filter = 0; filter < Variables::numResampleFilters; ++filter)
        {
            m_filters.add (new juce::IIRFilter);
            m_filters[filter]->setCoefficients (juce::IIRCoefficients::makeLowPass(newRate * static_cast<double> (Variables::resampleCoefficient), 20000.0));
        }
    }
}

void SynthesiserVoice::modulateFrequency (float modulationAmount)
{
    for (int voice = 0; voice < m_numActiveVoices; ++voice)
    {
        m_voices[voice]->setFrequency (m_frequency * modulationAmount * getDetuneAmount (voice));
    }
}

void SynthesiserVoice::pitchBendModulation()
{
    if (m_pitchBend > 0.f)
    {
        modulateFrequency (m_pitchBend + 1.f);
    }
    
    else
    {
        modulateFrequency (1.f / -(m_pitchBend - 1.f));
    }
}

float SynthesiserVoice::getDetuneAmount (int voice)
{
    float detune = 1.f;
    
    if (voice != 0)
    {
        detune = (static_cast<float> (voice + 1) / static_cast<float> (m_numVoices * 16)) + 1.f;
    }
    
    return detune;
}

void SynthesiserVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (m_playing)
    {
        juce::AudioBuffer<float> block (1, numSamples);
        juce::AudioBuffer<float> blockUpsample (1, numSamples * static_cast<float> (Variables::resampleCoefficient));
        block.clear();
        blockUpsample.clear();
        
        auto* blockUpsampleData = blockUpsample.getWritePointer (0);
        auto* blockData = block.getWritePointer (0);
        
        // Modulate in case of pitch bend action.
        if (m_pitchBend != 0.f || m_frequency != m_voices[0]->getFrequency())
        {
            pitchBendModulation();
        }
        
        // Increase sample rate to remove aliasing.
        for (int voice = 0; voice < m_numActiveVoices; ++voice)
        {
            m_voices[voice]->setSampleRate (m_voices[voice]->getSampleRate() * static_cast<float> (Variables::resampleCoefficient));
        }
        
        // Process samples.
        for (int voice = 0; voice < m_numActiveVoices; ++voice)
        {
            for (int sample = 0; sample < blockUpsample.getNumSamples(); ++sample)
            {
                blockUpsampleData[sample] += m_voices[voice]->processSample() * (1.f / static_cast<float> (m_numActiveVoices));
            }
        }
    
        // Filter upsampled signal.
        for (int filter = 0; filter < Variables::numResampleFilters; ++filter)
        {
            m_filters[filter]->processSamples (blockUpsampleData, blockUpsample.getNumSamples());
        }
        
        // Downsample signal and apply envelope.
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            blockData[sample] = blockUpsampleData[sample * Variables::resampleCoefficient] * (m_adsr.getNextSample() * m_adsr.getNextSample());
        }
        
        // Reset to original sample rate.
        for (int voice = 0; voice < m_numActiveVoices; ++voice)
        {
            m_voices[voice]->setSampleRate (m_voices[voice]->getSampleRate() / static_cast<float> (Variables::resampleCoefficient));
        }
        
        // Apply gain based on velocity.
        if (m_velocity != 1.f)
        {
            block.applyGain (m_velocity);
        }
        
        // Move audio data to output buffer.
        for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
        {
            outputBuffer.addFrom (channel, startSample, block, 0, 0, numSamples);
        }
        
        // Check if note is playing.
        if (!m_adsr.isActive())
        {
            clearCurrentNote();
            m_playing = false;
        }
    }
}
