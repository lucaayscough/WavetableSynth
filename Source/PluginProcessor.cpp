#include "Headers.h"


WavetableSynth::WavetableSynth()
    :
#ifndef JucePlugin_PreferredChannelConfigurations
        AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
        m_apvts (*this, nullptr, "Parameters", {
            // Wavetable.
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"wavetablePosition", 1}, "Wavetable Position", juce::NormalisableRange<float> (0, 255, 1), 0),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"numActiveVoices", 1}, "Num Active Voices", juce::NormalisableRange<float> (1, 16, 1), 1),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"detune", 1}, "Detune", 0.01f, 1.f, 0.f),
            
            // ADSR.
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"attack", 1}, "Attack", 0.01f, 4.f, 0.1f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"decay", 1}, "Decay", 0.1f, 4.f, 0.1f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"sustain", 1}, "Sustain", 0.f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"release", 1}, "Release", 0.1f, 4.f, 0.1f),
            
            // Filter.
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"filterCutoff", 1}, "Filter Cutoff", juce::NormalisableRange<float> (20.f, 20000.f, 1.f, 0.2f), 20000.f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"outputGain", 1}, "Output Gain", 0.f, 1.f, 0.5f),
        
            // Reverb.
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"reverbDryLevel", 1}, "Reverb Dry Level", 0.f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"reverbWetLevel", 1}, "Reverb Wet Level", 0.f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"reverbRoomSize", 1}, "Reverb Room Size", 0.f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"reverbWidth", 1}, "Reverb Width", 0.f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"reverbDamping", 1}, "Reverb Damping", 0.f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"reverbFreezeMode", 1}, "Reverb Freeze Mode", 0.f, 1.f, 0.5f)
        })
{
    // Init synth.
    m_synthesiser.addSound (new SynthesiserSound);
    
    for (int voice = 0; voice < m_numVoices; ++voice)
    {
        m_synthesiser.addVoice (new SynthesiserVoice);
    }
    
    // Plugin parameters.
    m_wavetablePosition = m_apvts.getRawParameterValue ("wavetablePosition");
    m_numActiveVoices = m_apvts.getRawParameterValue ("numActiveVoices");
    m_detune = m_apvts.getRawParameterValue ("detune");
    
    m_attack = m_apvts.getRawParameterValue ("attack");
    m_decay = m_apvts.getRawParameterValue ("decay");
    m_sustain = m_apvts.getRawParameterValue ("sustain");
    m_release = m_apvts.getRawParameterValue ("release");
    
    m_filterCutoff = m_apvts.getRawParameterValue ("filterCutoff");
    m_outputGain = m_apvts.getRawParameterValue ("outputGain");
    
    m_reverbDryLevel = m_apvts.getRawParameterValue ("reverbDryLevel");
    m_reverbWetLevel = m_apvts.getRawParameterValue ("reverbWetLevel");
    m_reverbRoomSize = m_apvts.getRawParameterValue ("reverbRoomSize");
    m_reverbWidth = m_apvts.getRawParameterValue ("reverbWidth");
    m_reverbDamping = m_apvts.getRawParameterValue ("reverbDamping");
    m_reverbFreezeMode = m_apvts.getRawParameterValue ("reverbFreezeMode");
}

WavetableSynth::~WavetableSynth() {}

//==============================================================================
const juce::String WavetableSynth::getName() const
{
    return JucePlugin_Name;
}

bool WavetableSynth::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynth::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WavetableSynth::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WavetableSynth::getTailLengthSeconds() const
{
    return 0.0;
}

int WavetableSynth::getNumPrograms() { return 1; }
int WavetableSynth::getCurrentProgram() { return 0; }
void WavetableSynth::setCurrentProgram (int index) {}
const juce::String WavetableSynth::getProgramName (int index) { return {}; }
void WavetableSynth::changeProgramName (int index, const juce::String& newName) {}

//==============================================================================
void WavetableSynth::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Prepare synth.
    m_synthesiser.setCurrentPlaybackSampleRate (sampleRate);
    
    // Prepare parameter smoothing.
    m_smoothWavetablePosition.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothWavetablePosition.setCurrentAndTargetValue (*m_wavetablePosition);
    
    m_smoothAttack.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothAttack.setCurrentAndTargetValue (*m_attack);
    
    m_smoothDecay.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothDecay.setCurrentAndTargetValue (*m_decay);
    
    m_smoothSustain.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothSustain.setCurrentAndTargetValue (*m_sustain);
    
    m_smoothRelease.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothRelease.setCurrentAndTargetValue (*m_release);
    
    m_smoothFilterCutoff.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothFilterCutoff.setCurrentAndTargetValue (*m_filterCutoff);
    
    m_smoothOutputGain.reset (sampleRate, Variables::parameterSmoothingTime);
    m_smoothOutputGain.setCurrentAndTargetValue (*m_outputGain);
    
    // Prepare filters.
    m_filters.clear();
    
    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        m_filters.add (new juce::IIRFilter);
        m_filters[channel]->reset();
    }
    
    // Prepare reverb.
    m_reverb.setSampleRate (sampleRate);
    m_reverb.reset();
}

void WavetableSynth::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WavetableSynth::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    buffer.clear();
    
    // Wavetable.
    m_smoothWavetablePosition.setTargetValue (*m_wavetablePosition);
    m_synthesiser.setWavetablePosition (m_smoothWavetablePosition.getNextValue());
    
    // ADSR.
    m_smoothAttack.setTargetValue (*m_attack);
    m_smoothDecay.setTargetValue (*m_decay);
    m_smoothSustain.setTargetValue (*m_sustain);
    m_smoothRelease.setTargetValue (*m_release);
    
    for (int voice = 0; voice < m_numVoices; ++voice)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*> (m_synthesiser.getVoice (voice));
        v->setEnvelope (m_smoothAttack.getNextValue(), m_smoothDecay.getNextValue(), m_smoothSustain.getNextValue(), m_smoothRelease.getNextValue());
        v->setNumActiveVoices (*m_numActiveVoices);
        v->setDetune (*m_detune);
    }
    
    // Render audio.
    m_synthesiser.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    
    // Filter.
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
    
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            m_smoothFilterCutoff.setTargetValue (*m_filterCutoff);
            m_filters[channel]->setCoefficients (juce::IIRCoefficients::makeLowPass (getSampleRate(), m_smoothFilterCutoff.getNextValue()));
            channelData[sample] = m_filters[channel]->processSingleSampleRaw (channelData[sample]);
        }
    }
    
    // Reverb.
    auto* leftChannel = buffer.getWritePointer (0);
    auto* rightChannel = buffer.getWritePointer (1);
    
    auto reverbParameters = m_reverb.getParameters();
    
    reverbParameters.dryLevel = *m_reverbDryLevel;
    reverbParameters.wetLevel = *m_reverbWetLevel;
    reverbParameters.roomSize = *m_reverbRoomSize;
    reverbParameters.width = *m_reverbWidth;
    reverbParameters.damping = *m_reverbDamping;
    reverbParameters.freezeMode = *m_reverbFreezeMode;
    
    m_reverb.setParameters (reverbParameters);
    
    m_reverb.processStereo (leftChannel, rightChannel, buffer.getNumSamples());

    // Gain.
    m_smoothOutputGain.setTargetValue (*m_outputGain);
    buffer.applyGain (m_smoothOutputGain.getNextValue());
}

bool WavetableSynth::hasEditor() const
{
    return false;
}

juce::AudioProcessorEditor* WavetableSynth::createEditor()
{
    return new WavetableSynthEditor (*this);
}

void WavetableSynth::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = m_apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void WavetableSynth::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
     
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (m_apvts.state.getType()))
            m_apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavetableSynth();
}
