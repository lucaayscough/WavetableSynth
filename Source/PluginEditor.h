#pragma once


class WavetableSynthEditor  : public juce::AudioProcessorEditor
{
public:
    WavetableSynthEditor (WavetableSynth&);
    ~WavetableSynthEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    WavetableSynth& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthEditor)
};
