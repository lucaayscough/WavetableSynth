#include "Headers.h"


WavetableSynthEditor::WavetableSynthEditor (WavetableSynth& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);
}

WavetableSynthEditor::~WavetableSynthEditor() {}

void WavetableSynthEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void WavetableSynthEditor::resized() {}
