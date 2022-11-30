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
    View m_view;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynthEditor)
};
