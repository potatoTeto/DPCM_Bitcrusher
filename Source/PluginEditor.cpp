#include "PluginEditor.h"

DPCMBitcrusherAudioProcessorEditor::DPCMBitcrusherAudioProcessorEditor(DPCMBitcrusherAudioProcessor& p, juce::AudioProcessorValueTreeState& state)
    : AudioProcessorEditor(&p), processorRef(p), apvts(state)
{
    _inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "inputGain", _inputGainSlider);
    _outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "gain", _outputGainSlider);
    _qualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "quality", _qualityComboBox);
    _bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        apvts, "bypass", _bypassToggle);

    addAndMakeVisible(_inputGainSlider);
    _inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    addAndMakeVisible(_inputGainLabel);

    addAndMakeVisible(_outputGainSlider);
    _outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    addAndMakeVisible(_outputGainLabel);

    _mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "mix", _mixSlider);
    addAndMakeVisible(_mixSlider);
    _mixLabel.setText("Mix Level", juce::dontSendNotification);
    addAndMakeVisible(_mixLabel);

    // Add ComboBox IDs 0..15 to exactly match quality parameter values
    for (int i = 0; i <= 15; ++i)
        _qualityComboBox.addItem("Quality " + juce::String(i), i + 1);

    // Create attachment (syncs ComboBox with parameter)
    _qualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts, "quality", _qualityComboBox);
    
    // Force ComboBox refresh (optional but can fix display issues)
    _qualityComboBox.setSelectedId(_qualityComboBox.getSelectedId(), juce::dontSendNotification);

    addAndMakeVisible(_qualityComboBox);
    _qualityLabel.setText("Quality", juce::dontSendNotification);
    addAndMakeVisible(_qualityLabel);

    addAndMakeVisible(_bypassToggle);
    _bypassToggle.setButtonText("Bypass");

    setSize(400, 350);
}

DPCMBitcrusherAudioProcessorEditor::~DPCMBitcrusherAudioProcessorEditor() {}

void DPCMBitcrusherAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("NES DPCM Bitcrusher v1.1 by potatoTeto", getLocalBounds(), juce::Justification::centredTop, 1);
}

void DPCMBitcrusherAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto rowHeight = 40;
    const int spacing = 10;

    area.removeFromTop(20);

    _inputGainLabel.setBounds(area.removeFromTop(20));
    _inputGainSlider.setBounds(area.removeFromTop(rowHeight));

    _outputGainLabel.setBounds(area.removeFromTop(20));
    _outputGainSlider.setBounds(area.removeFromTop(rowHeight));

    _qualityLabel.setBounds(area.removeFromTop(20));
    _qualityComboBox.setBounds(area.removeFromTop(rowHeight));

    area.removeFromTop(spacing);
    _mixLabel.setBounds(area.removeFromTop(20));
    _mixSlider.setBounds(area.removeFromTop(rowHeight));

    _bypassToggle.setBounds(area.removeFromTop(rowHeight));
}
