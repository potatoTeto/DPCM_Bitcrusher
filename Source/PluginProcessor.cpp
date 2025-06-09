#include "PluginProcessor.h"
#include "PluginEditor.h"

DPCMBitcrusherAudioProcessor::DPCMBitcrusherAudioProcessor()
    : AudioProcessor(BusesProperties()
                        .withInput("Input", juce::AudioChannelSet::stereo(), true)
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

DPCMBitcrusherAudioProcessor::~DPCMBitcrusherAudioProcessor() {}

const juce::String DPCMBitcrusherAudioProcessor::getName() const { return JucePlugin_Name; }

bool DPCMBitcrusherAudioProcessor::acceptsMidi() const { return false; }
bool DPCMBitcrusherAudioProcessor::producesMidi() const { return false; }
bool DPCMBitcrusherAudioProcessor::isMidiEffect() const { return false; }
double DPCMBitcrusherAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int DPCMBitcrusherAudioProcessor::getNumPrograms() { return 1; }
int DPCMBitcrusherAudioProcessor::getCurrentProgram() { return 0; }
void DPCMBitcrusherAudioProcessor::setCurrentProgram(int) {}
const juce::String DPCMBitcrusherAudioProcessor::getProgramName(int) { return {}; }
void DPCMBitcrusherAudioProcessor::changeProgramName(int, const juce::String&) {}

void DPCMBitcrusherAudioProcessor::prepareToPlay(double, int) {}
void DPCMBitcrusherAudioProcessor::releaseResources() {}

void DPCMBitcrusherAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const bool isBypassed = apvts.getRawParameterValue("bypass")->load() > 0.5f;
    if (isBypassed)
        return;

    float inputGain = apvts.getRawParameterValue("inputGain")->load();
    float outputGain = apvts.getRawParameterValue("gain")->load();
    int quality = static_cast<int>(apvts.getRawParameterValue("quality")->load());

    const int maxDeltaSteps = 64;  // NES DPCM delta steps
    const float stepSize = 1.0f / (float)maxDeltaSteps;
    const int stepInterval = 1 << (15 - quality);

    const float deadZone = stepSize * 0.5f;  // Half-step dead zone

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        auto* data = buffer.getWritePointer(channel);
        float accumulator = 0.0f;
        int sampleIndex = 0;

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float sample = data[i] * inputGain;

            if (sampleIndex == 0)
            {
                float delta = sample - accumulator;

                // Only update accumulator if delta is larger than deadZone
                if (std::abs(delta) >= deadZone)
                {
                    float step = (delta >= 0.0f) ? stepSize : -stepSize;
                    accumulator += step;

                    // Clamp accumulator between -1 and 1
                    accumulator = juce::jlimit(-1.0f, 1.0f, accumulator);
                }
                // else accumulator stays the same, avoiding jitter

                data[i] = accumulator * outputGain;
            }
            else
            {
                // Hold previous accumulator value during skipped samples
                data[i] = accumulator * outputGain;
            }

            sampleIndex = (sampleIndex + 1) % stepInterval;
        }
    }
}

bool DPCMBitcrusherAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* DPCMBitcrusherAudioProcessor::createEditor()
{
    return new DPCMBitcrusherAudioProcessorEditor(*this, apvts);
}

void DPCMBitcrusherAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    if (state.isValid())
    {
        std::unique_ptr<juce::XmlElement> xml(state.createXml());
        copyXmlToBinary(*xml, destData);
    }
}


void DPCMBitcrusherAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState)
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout DPCMBitcrusherAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("inputGain", "Input Gain", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("gain", "Output Gain", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterInt>("quality", "Quality", 0, 15, 15));
    params.push_back(std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));

    return { params.begin(), params.end() };
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DPCMBitcrusherAudioProcessor();
}
