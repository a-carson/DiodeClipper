/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
ClippingNonlinearitiesAudioProcessor::ClippingNonlinearitiesAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),

#endif
    parameters(*this, nullptr, "ParamTreeIdentifier", {
    std::make_unique < AudioParameterFloat >("gain", "Gain", 0.0f, 50.0f , 0.5f) ,
    std::make_unique < AudioParameterFloat >("output", "Level", 0.0f, 1.0f, 0.5f),
        })
{
    gain = parameters.getRawParameterValue("gain");
    out = parameters.getRawParameterValue("output");
}

ClippingNonlinearitiesAudioProcessor::~ClippingNonlinearitiesAudioProcessor()
{
}

//==============================================================================
const String ClippingNonlinearitiesAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ClippingNonlinearitiesAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ClippingNonlinearitiesAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ClippingNonlinearitiesAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ClippingNonlinearitiesAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ClippingNonlinearitiesAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ClippingNonlinearitiesAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ClippingNonlinearitiesAudioProcessor::setCurrentProgram (int index)
{
}

const String ClippingNonlinearitiesAudioProcessor::getProgramName (int index)
{
    return {};
}

void ClippingNonlinearitiesAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ClippingNonlinearitiesAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    // initialise
    diodeClipper.setSampleRate(sampleRate);
    diodeClipper.setCircuitParameters(R, C);
    diodeClipper.setDiodeParameters(Is, Vt, Ni);
    diodeClipper.initialise();

    sineOsc.setSampleRate(sampleRate);
    sineOsc.setFrequency(100.0f);
}

void ClippingNonlinearitiesAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ClippingNonlinearitiesAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void ClippingNonlinearitiesAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    int numSamples = buffer.getNumSamples();
    float* left = buffer.getWritePointer(0);
    float* right = buffer.getWritePointer(1);

    // tanh clipper -----------------------------
    //for (int i = 0; i < numSamples; i++)
    //{
    //    left[i] = *out * tanh(*gain * left[i]);
    //    right[i] = *out * tanh(*gain * right[i]);
    //}

    // diode clipper
    for (int i = 0; i < numSamples; i++)
    {
        //left[i] = sineOsc.process();
        //right[i] = sineOsc.process();
        left[i] = *out * diodeClipper.process(*gain * left[i]);
        right[i] = left[i];
    }


}

//==============================================================================
bool ClippingNonlinearitiesAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ClippingNonlinearitiesAudioProcessor::createEditor()
{
    return new ClippingNonlinearitiesAudioProcessorEditor (*this);
}

//==============================================================================
void ClippingNonlinearitiesAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ClippingNonlinearitiesAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ClippingNonlinearitiesAudioProcessor();
}
