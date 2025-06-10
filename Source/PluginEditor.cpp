#include "PluginEditor.h"

DPCMBitcrusherAudioProcessorEditor::DPCMBitcrusherAudioProcessorEditor(DPCMBitcrusherAudioProcessor& p, juce::AudioProcessorValueTreeState& state)
    : AudioProcessorEditor(&p), processorRef(p), apvts(state)
{
    _inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "inputGain", _inputGainSlider);
    _outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "gain", _outputGainSlider);
    _sampleUpdateRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "sampleUpdateRate", _sampleUpdateRateSlider);
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

    // Add sampleUpdateRate slider that snaps to integers
    _sampleUpdateRateSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    _sampleUpdateRateSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    _sampleUpdateRateSlider.setRange(0, 15, 1); // Snap to integers
    _sampleUpdateRateSlider.setValue(15); // Default value
    _sampleUpdateRateSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(_sampleUpdateRateSlider);
    _sampleUpdateRateLabel.setText("Sample/Delta Update Rate", juce::dontSendNotification);
    addAndMakeVisible(_sampleUpdateRateLabel);

    _sampleUpdateRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "sampleUpdateRate", _sampleUpdateRateSlider);

    // Dynamically update the label with stepInterval
    _sampleUpdateRateSlider.onValueChange = [this]()
    {
        int q = static_cast<int>(_sampleUpdateRateSlider.getValue());
        int stepInterval = 1 << (15 - q);
        _sampleUpdateRateLabel.setText("Sample/Delta Update Rate: Every " + juce::String(stepInterval) + " samples)", juce::dontSendNotification);
    };
    _sampleUpdateRateSlider.onValueChange(); // Trigger once to initialize

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
    g.drawFittedText("NES DPCM Bitcrusher v1.2 by potatoTeto", getLocalBounds(), juce::Justification::centredTop, 1);
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
    
    area.removeFromTop(20);
    
    _sampleUpdateRateLabel.setBounds(area.removeFromTop(20));
    _sampleUpdateRateSlider.setBounds(area.removeFromTop(rowHeight));

    area.removeFromTop(spacing);
    _mixLabel.setBounds(area.removeFromTop(20));
    _mixSlider.setBounds(area.removeFromTop(rowHeight));

    _bypassToggle.setBounds(area.removeFromTop(rowHeight));
}
