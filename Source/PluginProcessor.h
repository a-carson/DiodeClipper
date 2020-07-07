/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>
#include "DiodeClipper.h"
#include "Oscillator.h"
//#include "juce_dsp/maths/juce_FastMathApproximations.h"

//==============================================================================
/**
*/
class ClippingNonlinearitiesAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ClippingNonlinearitiesAudioProcessor();
    ~ClippingNonlinearitiesAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    AudioProcessorValueTreeState parameters;
    std::atomic <float>* gain;
    std::atomic <float>* out;
    DiodeClipper<float> diodeClipper;
    float R = 1000.0f;
    float C = 33e-9;
    float Is = 2.52e-9; 
    float Vt = 25.83e-3;
    float Ni = 1.752;
    SineOsc sineOsc;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ClippingNonlinearitiesAudioProcessor)
};
