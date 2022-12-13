#pragma once


class WavetableSynth  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    WavetableSynth();
    ~WavetableSynth() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::AudioProcessorValueTreeState m_apvts;
    Synthesiser m_synthesiser;
    const int m_numVoices = 16;
    
    // DSP.
    juce::OwnedArray<juce::IIRFilter> m_filters;
    juce::Reverb m_reverb;
    
    // Global parameters.
    std::atomic<float>* m_wavetablePosition;
    juce::SmoothedValue<float> m_smoothWavetablePosition;
    
    std::atomic<float>* m_attack;
    std::atomic<float>* m_decay;
    std::atomic<float>* m_sustain;
    std::atomic<float>* m_release;
    juce::SmoothedValue<float> m_smoothAttack;
    juce::SmoothedValue<float> m_smoothDecay;
    juce::SmoothedValue<float> m_smoothSustain;
    juce::SmoothedValue<float> m_smoothRelease;

    std::atomic<float>* m_filterCutoff;
    juce::SmoothedValue<float> m_smoothFilterCutoff;
    
    std::atomic<float>* m_reverbDryLevel;
    std::atomic<float>* m_reverbWetLevel;
    std::atomic<float>* m_reverbRoomSize;
    std::atomic<float>* m_reverbWidth;
    std::atomic<float>* m_reverbDamping;
    std::atomic<float>* m_reverbFreezeMode;
    
    std::atomic<float>* m_outputGain;
    juce::SmoothedValue<float> m_smoothOutputGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynth)
};
