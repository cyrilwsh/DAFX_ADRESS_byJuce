/*
  This code accompanies the textbook:
 
  Digital Audio Effects: Theory, Implementation and Application
  Joshua D. Reiss and Andrew P. McPherson
 
  ---
 
  Parametric EQ: parametric equaliser adjusting frequency, Q and gain
  See textbook Chapter 4: Filter Effects
 
  Code by Andrew McPherson, Brecht De Man and Joshua Reiss
 
  ---

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PLUGINPROCESSOR_H_4693CB6E__
#define __PLUGINPROCESSOR_H_4693CB6E__

#define _USE_MATH_DEFINES
#include "../JuceLibraryCode/JuceHeader.h"
//#include "sourceSeparationADRessFilter.h"

//==============================================================================
/**
*/
class sourceSeparationADRessAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    sourceSeparationADRessAudioProcessor();
    ~sourceSeparationADRessAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;
    bool acceptsMidi() const;
    bool producesMidi() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    //==============================================================================
    
    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth_, lastUIHeight_;
    
    enum Parameters
    {
        kgBetaParam = 0, /* Centre frequency in Hz */
        kQParam,                   /* Filter Q */
        kgain1DecibelsParam,        /* Boost/cut in decibels */
        kgain2DecibelsParam,
        kpositionChoiceParam,
        klowRatioParam,
        kAZsubplaceWidthParam,
        klowReleaseParam,
        klowGainParam,
        khighThresholdParam,
        khighRatioParam,
        khighAttackParam,
        khighReleaseParam,
        khighGainParam,
        kband01GainParam,
        kband02GainParam,
        kband03GainParam,
        kband04GainParam,
        kband05GainParam,
        kband06GainParam,
        kband07GainParam,
        kband08GainParam,
        kband09GainParam,
        kband10GainParam,
        kband11GainParam,
        kNumParameters
    };
    
    float gBeta_, q_, gain1Decibels_, gain2Decibels_;
    
    // Adjustable parameters:
    float freqLow_;
    float freqHigh_;
    float positionChoice_ = 0 ;
    float lowRatio_;
    float AZsubplaceWidth_ = 1;
    float lowRelease_;
    float lowGain_;
    float highThreshold_;
    float highRatio_;
    float highAttack_;
    float highRelease_;
    float highGain_;
    
    // band EQ
    float band01Gain_ = 0;
    float band02Gain_ = 0;
    float band03Gain_ = 0;
    float band04Gain_ = 0;
    float band05Gain_ = 0;
    float band06Gain_ = 0;
    float band07Gain_ = 0;
    float band08Gain_ = 0;
    float band09Gain_ = 0;
    float band10Gain_ = 0;
    float band11Gain_ = 0;
    
    float band01max = 0;
    float band02max = 0;
    float band03max = 0;
    float band04max = 0;
    float band05max = 0;
    float band06max = 0;
    float band07max = 0;
    float band08max = 0;
    float band09max = 0;
    float band10max = 0;
    float band11max = 0;
    
    
    float sampleRate = 44100;
    
    
    
    // shw add for filter
    float gLPFb0, gLPFb1, gLPFb2, gLPFa1, gLPFa2;
    float gHPFb0, gHPFb1, gHPFb2, gHPFa1, gHPFa2;
    float gPreIn, gPrePreIn, gLPFPreOut, gLPFPrePreOut, gHPFPreOut, gHPFPrePreOut;
    float gLPFPreLPFOut1;
    float gLPFPrePreLPFOut1;
    float gLPFPreLPFOut2;
    float gLPFPrePreLPFOut2;
    
    float gHPFPreHPFOut1;
    float gHPFPrePreHPFOut1;
    float gHPFPreHPFOut2;
    float gHPFPrePreHPFOut2;

    //************ fft *************
    int windowSize = 8192;
    
    float windowTriangle[8192] = {0};
    
    // Left
    std::complex<float> fftBufferL0[8192] = {0};
    std::complex<float> fftBufferL1[8192] = {0};
    int fftBufferL0writeIndex = 0;
    int fftBufferL1writeIndex = (fftBufferL0writeIndex + 4096) % 8192;
    int fftBufferL0cReadIndex;
    int fftBufferL1cReadIndex;
    
    std::complex<float> fftFreqL0a[8192] = {0};
    std::complex<float> fftFreqL0b[8192] = {0};
    std::complex<float> fftFreqL0c[8192] = {0};
    
    std::complex<float> fftFreqL1a[8192] = {0};
    std::complex<float> fftFreqL1b[8192] = {0};
    std::complex<float> fftFreqL1c[8192] = {0};
    
    std::complex<float> fftOutL0[8192] = {0};
    std::complex<float> fftOutL1[8192] = {0};
    
    // Right
    std::complex<float> fftBufferR0[8192] = {0};
    std::complex<float> fftBufferR1[8192] = {0};
    int fftBufferR0writeIndex = 0;
    int fftBufferR1writeIndex = (fftBufferR0writeIndex + 4096) % 8192;
    int fftBufferR0cReadIndex;
    int fftBufferR1cReadIndex;
    
    std::complex<float> fftFreqR0a[8192] = {0};
    std::complex<float> fftFreqR0b[8192] = {0};
    std::complex<float> fftFreqR0c[8192] = {0};
    
    std::complex<float> fftFreqR1a[8192] = {0};
    std::complex<float> fftFreqR1b[8192] = {0};
    std::complex<float> fftFreqR1c[8192] = {0};
    
    std::complex<float> fftOutR0[8192] = {0};
    std::complex<float> fftOutR1[8192] = {0};
    
    // use circular buffer
    const int gBufferSize = 16384;
    float gInputBufferL[ 16384 ] = {0};
    float gInputBufferR[ 16384 ] = {0};
    float gOutputBufferL[16384] = {0};
    float gOutputBufferR[16384] = {0};
    
    int gFFTSize = 8192;
    int gHopSize = 2048;
    int gHopCounter = 0;
    
//    float gInputBufferL[ 8192 ] = {0};
//    float gInputBufferR[ 8192 ] = {0};

    
    int gInputBufferPointer = 0;
    
    

    
    int gOutputBufferWritePointer = gHopSize;
    int gOutputBufferReadPointer = 0;
    
    float gWindowBuffer[8192];
    
    std::complex<float> timeDomainInL  [ 8192 ] = {0};
    std::complex<float> timeDomainInR  [ 8192 ] = {0};
    std::complex<float> freqDomainInL  [ 8192 ] = {0};
    std::complex<float> freqDomainInR  [ 8192 ] = {0};
    std::complex<float> freqDomainOutL [ 8192 ] = {0};
    std::complex<float> freqDomainOutR [ 8192 ] = {0};
    std::complex<float> timeDomainOutL [ 8192 ] = {0};
    std::complex<float> timeDomainOutR [ 8192 ] = {0};

    
    
    
#include <tuple>
    
    dsp::FFT dsp_fft{13};   // 13bit = 8192
    
//    float* makeTriangleWindow(int size);
    
//    dsp::FFT ForwardFFT(9);
//    dsp::FFT InverseFFT(9);
    
    std::tuple<double, double, double, double, double> calLPFCoeff(double sampleRate, double cutoffFrequency);
    std::tuple<double, double, double, double, double> calHPFCoeff(double sampleRate, double cutoffFrequency);
    
    
private:
//    void updateEQFilter(float sampleRate);
    
//    sourceSeparationADRessFilter **eqFilters_;
    int numEqFilters_;
    
    
    void lowCompression(int numSamples);
    void highCompression(int numSamples);
    IIRFilter *lowFilterL_, *midFilterL_, *highFilterL_, *lowFilterR_, *midFilterR_, *highFilterR_;
     AudioSampleBuffer lowBuffer_, midBuffer_, highBuffer_, inputBuffer_;
    
    // Compression function
    float yLprevLow_, yLprevHigh_;
    HeapBlock <float> xg_, xl_, yg_, yl_, c_;
    
    void algoADRessL0();
    void algoADRessL1();
    void algoADRess();
    float bandGainCalculation(float binNum, int fftNumber, float magnitude);
    
    enum{
        kState0 = 0,
        kState1
    };
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (sourceSeparationADRessAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_4693CB6E__
