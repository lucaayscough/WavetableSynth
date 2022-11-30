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
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"attack", 1}, "Attack", 0.1f, 4.f, 0.1f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"decay", 1}, "Decay", 0.1f, 4.f, 0.1f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"sustain", 1}, "Sustain", 0.1f, 1.f, 0.5f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"release", 1}, "Release", 0.1f, 4.f, 0.1f),
            
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"filterCutoff", 1}, "Filter Cutoff", juce::NormalisableRange<float> (20.f, 20000.f, 1.f, 0.2f), 20000.f),
            std::make_unique<juce::AudioParameterFloat> (juce::ParameterID {"outputGain", 1}, "Output Gain", 0.f, 1.f, 0.5f)
        })
{
    // Init synth.
    m_synthesiser.addSound (new SynthesiserSound);
    
    for (int voice = 0; voice < m_numVoices; ++voice)
    {
        m_synthesiser.addVoice (new SynthesiserVoice);
    }
    
    // Plugin parameters.
    m_attack = m_apvts.getRawParameterValue ("attack");
    m_decay = m_apvts.getRawParameterValue ("decay");
    m_sustain = m_apvts.getRawParameterValue ("sustain");
    m_release = m_apvts.getRawParameterValue ("release");
    
    m_filterCutoff = m_apvts.getRawParameterValue ("filterCutoff");
    m_outputGain = m_apvts.getRawParameterValue ("outputGain");
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
    m_synthesiser.setCurrentPlaybackSampleRate (sampleRate);
    
    m_smoothAttack.reset (sampleRate, 0.0001);
    m_smoothDecay.reset (sampleRate, 0.0001);
    m_smoothSustain.reset (sampleRate, 0.0001);
    m_smoothRelease.reset (sampleRate, 0.0001);
    
    m_smoothFilterCutoff.reset (sampleRate, 0.01);
    m_smoothOutputGain.reset (sampleRate, 0.0001);
    
    for (int channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        m_filters.add (new juce::IIRFilter);
    }
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
    
    // ADSR.
    m_smoothAttack.setTargetValue (*m_attack);
    m_smoothDecay.setTargetValue (*m_decay);
    m_smoothSustain.setTargetValue (*m_sustain);
    m_smoothRelease.setTargetValue (*m_release);
    
    for (int voice = 0; voice < m_numVoices; ++voice)
    {
        SynthesiserVoice* v = dynamic_cast<SynthesiserVoice*>(m_synthesiser.getVoice (voice));
        v->setEnvelope (m_smoothAttack.getNextValue(), m_smoothDecay.getNextValue(), m_smoothSustain.getNextValue(), m_smoothRelease.getNextValue());
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
