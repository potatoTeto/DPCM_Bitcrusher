#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DPCMBitcrusherAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DPCMBitcrusherAudioProcessorEditor(DPCMBitcrusherAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~DPCMBitcrusherAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    DPCMBitcrusherAudioProcessor& processorRef;
    juce::AudioProcessorValueTreeState& apvts;

    juce::Slider _inputGainSlider;
    juce::Label _inputGainLabel;

    juce::Slider _outputGainSlider;
    juce::Label _outputGainLabel;

    juce::ComboBox _qualityComboBox;
    juce::Label _qualityLabel;

    juce::ToggleButton _bypassToggle;

    juce::Slider _mixSlider;
    juce::Label _mixLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _outputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> _qualityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> _bypassAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> _mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DPCMBitcrusherAudioProcessorEditor)
};
