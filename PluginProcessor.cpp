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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "iostream" // for std tupple

//==============================================================================
sourceSeparationADRessAudioProcessor::sourceSeparationADRessAudioProcessor()
{
    // Set default values:
    gBeta_ = 3;
    
    positionChoice_ = 1 ;
    
    q_ = 2.0;
    gain1Decibels_ = 0.0;
    gain2Decibels_ = 0.0;
    lowGain_ = 0.0;
    
    // shw add, initialise global variable for filter
    gLPFb0 =0.0;    // initial test
    gLPFb1 =0.0;
    gLPFb2 =0.0;
    gLPFa1 =0.0;
    gLPFa2 =0.0;
    gHPFb1 =0.0;
    gHPFb2 =0.0;
    gHPFa1 =0.0;
    gHPFa2 =0.0;
    
    gPreIn=0.0;
    gPrePreIn=0.0;
    gLPFPreOut=0.0;
    gLPFPrePreOut=0.0;
    gHPFPreOut=0.0;
    gHPFPrePreOut=0.0;
    
    gLPFPreLPFOut1=0.0;
    gLPFPrePreLPFOut1=0.0;
    gLPFPreLPFOut2=0.0;
    gLPFPrePreLPFOut2=0.0;
    
    // Initialise the filters later when we know how many channels
//    eqFilters_ = 0;
    numEqFilters_ = 0;
    
//    lastUIWidth_ = 550;
//    lastUIHeight_ = 100;
    lastUIWidth_ = 400;
    lastUIHeight_ = 600;

    yLprevLow_ = 0.0;
    
    yLprevHigh_ = 0.0;
    
    // build a 8192 triangle window
    float size = 8192;
    for (int i = 0; i < size; i++){
        if (i <= size/2)
            windowTriangle[i] = float(i) / float( size/2 );
        else
            windowTriangle[i] = 1 - float( i - size/2 ) / float(size/2);
    }
    
    // Calculate a Hamming window
    for(int n = 0; n < gFFTSize; n++) {
        gWindowBuffer[n] = 0.46164f - ( 1.0 - 0.46164 ) * ( cosf(2.0 * M_PI * n / (float)(gFFTSize - 1)));
    }
    
}

sourceSeparationADRessAudioProcessor::~sourceSeparationADRessAudioProcessor()
{
}

//==============================================================================
const String sourceSeparationADRessAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int sourceSeparationADRessAudioProcessor::getNumParameters()
{
    return kNumParameters;
}

float sourceSeparationADRessAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case kgBetaParam: return gBeta_;
        case kQParam:               return q_;
        case kgain1DecibelsParam:    return gain1Decibels_;
        case kgain2DecibelsParam:    return gain2Decibels_;
        default:                    return 0.0f;
    }
}

void sourceSeparationADRessAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case kgBetaParam:
            gBeta_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        case kQParam:
            q_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        case kgain1DecibelsParam:
            gain1Decibels_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        case kgain2DecibelsParam:
            gain2Decibels_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        case kpositionChoiceParam:
            positionChoice_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        case klowRatioParam:
            lowRatio_ = newValue;
            //updateEQFilter(getSampleRate());
            break;

        case kAZsubplaceWidthParam:
            AZsubplaceWidth_ = newValue;
            //updateEQFilter(getSampleRate());
            break;

        case klowReleaseParam:
            lowRelease_ = newValue;
            //updateEQFilter(getSampleRate());
            break;

        case klowGainParam:
            lowGain_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
            
        case khighThresholdParam:
            highThreshold_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        case khighRatioParam:
            highRatio_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
            
        case khighAttackParam:
            highAttack_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
            
        case khighReleaseParam:
            highRelease_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
            
        case khighGainParam:
            highGain_ = newValue;
            //updateEQFilter(getSampleRate());
            break;
        
        case kband01GainParam: band01Gain_ = newValue; break;
        case kband02GainParam: band02Gain_ = newValue; break;
        case kband03GainParam: band03Gain_ = newValue; break;
        case kband04GainParam: band04Gain_ = newValue; break;
        case kband05GainParam: band05Gain_ = newValue; break;
        case kband06GainParam: band06Gain_ = newValue; break;
        case kband07GainParam: band07Gain_ = newValue; break;
        case kband08GainParam: band08Gain_ = newValue; break;
        case kband09GainParam: band09Gain_ = newValue; break;
        case kband10GainParam: band10Gain_ = newValue; break;
        case kband11GainParam: band11Gain_ = newValue; break;

            
        default:
            break;
    }
}

const String sourceSeparationADRessAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case kgBetaParam:  return "Beta";
        case kQParam:                return "Q";
        case kgain1DecibelsParam:     return "gain1 (dB)";
        case kgain2DecibelsParam:     return "gain2 (dB)";
        default:                     break;
    }
    
    return String::empty;
}

const String sourceSeparationADRessAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String sourceSeparationADRessAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String sourceSeparationADRessAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool sourceSeparationADRessAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool sourceSeparationADRessAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool sourceSeparationADRessAudioProcessor::silenceInProducesSilenceOut() const
{
#if JucePlugin_SilenceInProducesSilenceOut
    return true;
#else
    return false;
#endif
}

double sourceSeparationADRessAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

bool sourceSeparationADRessAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool sourceSeparationADRessAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int sourceSeparationADRessAudioProcessor::getNumPrograms()
{
    return 0;
}

int sourceSeparationADRessAudioProcessor::getCurrentProgram()
{
    return 0;
}

void sourceSeparationADRessAudioProcessor::setCurrentProgram (int index)
{
}

const String sourceSeparationADRessAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void sourceSeparationADRessAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void sourceSeparationADRessAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    // Create as many filters as we have input channels
    numEqFilters_ = getTotalNumInputChannels();
//    eqFilters_ = (sourceSeparationADRessFilter**)malloc(numEqFilters_ * sizeof(sourceSeparationADRessFilter*));
//    if(eqFilters_ == 0)
//        numEqFilters_ = 0;
//    else {
//        for(int i = 0; i < numEqFilters_; i++)
//            eqFilters_[i] = new sourceSeparationADRessFilter;
//    }
    
    
    // Set the size of the helper buffers
    lowBuffer_.setSize(getTotalNumInputChannels(), samplesPerBlock);
    highBuffer_.setSize(getTotalNumInputChannels(), samplesPerBlock);
//    inputBuffer_.setSize(getTotalNumInputChannels(), samplesPerBlock);
    
    // Set the size of the helper arrays
    xg_.allocate(samplesPerBlock, true);
    xl_.allocate(samplesPerBlock, true);
    yg_.allocate(samplesPerBlock, true);
    yl_.allocate(samplesPerBlock, true);
    c_.allocate(samplesPerBlock, true);
    
    
    // Update the filter settings to work with the current parameters and sample rate
    //updateEQFilter(sampleRate);
}

void sourceSeparationADRessAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
//    for(int i = 0; i < numEqFilters_; i++)
//        delete eqFilters_[i];
//    if(numEqFilters_ != 0)
//        free(eqFilters_);
//    numEqFilters_ = 0;
//    eqFilters_ = 0;
}


/* --------------------processBlock----------------- */

void sourceSeparationADRessAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // Helpful information about this block of samples:
//    const int totalNumInputChannels = getTotalNumInputChannels();     // How many input channels for our effect?
    const int TotalNumOutputChannels = getTotalNumOutputChannels();   // How many output channels for our effect?
    const int numSamples = buffer.getNumSamples();          // How many samples in the buffer for this block?
    int channel;
    sampleRate = getSampleRate();
    
    // channelData is an array of length numSamples which contains the audio for one channel
    float* channelDataL = buffer.getWritePointer(0); // only process one channel
    float* channelDataR = buffer.getWritePointer(1); // only process one channel
    
    // Core function
    for (int i = 0; i < numSamples; ++i)
    {
        const float inL = channelDataL[i];
        const float inR = channelDataR[i];
        
        gInputBufferL[ gInputBufferPointer] = inL;
        gInputBufferR[ gInputBufferPointer] = inR;
        
        if ( ++ gInputBufferPointer >= gBufferSize) // BUFFER_SIZE: 8192
            gInputBufferPointer = 0;
        
        // **************** Synthesis *********************
        // L channel : sum up 2 way triangle window
        channelDataL[i] = pow(10, lowGain_/20) * ( gOutputBufferL [gOutputBufferReadPointer] );
        channelDataR[i] = pow(10, lowGain_/20) * ( gOutputBufferR [gOutputBufferReadPointer] );
        // Then clear the output sample in the buffer so it is ready for the next overlap-add
        gOutputBufferL[ gOutputBufferReadPointer ] = 0;
        gOutputBufferR[ gOutputBufferReadPointer ] = 0;
        // Increment the read pointer in the output cicular buffer
        gOutputBufferReadPointer++;
        if(gOutputBufferReadPointer >= gBufferSize){
            gOutputBufferReadPointer = 0;
        }
        
        // HOP. Do ADRess every HOP size, Overlap-Add
        gHopCounter++;
        if(gHopCounter >= gHopSize) {
            gHopCounter = 0;
            // do Algorithm ADRess, include FFT & IFFT
            // debug
            algoADRess();
            gOutputBufferWritePointer = (gOutputBufferWritePointer + gHopSize) % gBufferSize;
            
            
            
            
        }
        /*
        */
        

    }   // end i (numSample)
    
    // clear the rest of the buffer values
    channel = 2;
        while(channel < TotalNumOutputChannels){
        buffer.clear (channel++, 0, buffer.getNumSamples());
    }   // end while
    
    
}

//==============================================================================
bool sourceSeparationADRessAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* sourceSeparationADRessAudioProcessor::createEditor()
{
    return new sourceSeparationADRessAudioProcessorEditor (this);
}

//==============================================================================
void sourceSeparationADRessAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Create an outer XML element..
    XmlElement xml("C4DMPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute("uiWidth", lastUIWidth_);
    xml.setAttribute("uiHeight", lastUIHeight_);
    xml.setAttribute("gBeta", gBeta_);
    xml.setAttribute("q", q_);
    xml.setAttribute("gain1Decibels", gain1Decibels_);
    xml.setAttribute("gain2Decibels", gain2Decibels_);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary(xml, destData);
}

void sourceSeparationADRessAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if(xmlState != 0)
    {
        // make sure that it's actually our type of XML object..
        if(xmlState->hasTagName("C4DMPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth_  = xmlState->getIntAttribute("uiWidth", lastUIWidth_);
            lastUIHeight_ = xmlState->getIntAttribute("uiHeight", lastUIHeight_);
            
            gBeta_ = (float)xmlState->getDoubleAttribute("gBeta", gBeta_);
            q_ = (float)xmlState->getDoubleAttribute("q", q_);
            gain1Decibels_ = (float)xmlState->getDoubleAttribute("gain1Decibels", gain1Decibels_);
            gain2Decibels_ = (float)xmlState->getDoubleAttribute("gain2Decibels", gain2Decibels_);
            //updateEQFilter(getSampleRate());
        }
    }
}

//==============================================================================


// use circular buffer
void sourceSeparationADRessAudioProcessor::algoADRess()
{
    
    // prepare to do FFT, move the pointer one FFTSize before now
    int pointer = ( gInputBufferPointer - gFFTSize + gBufferSize ) % gBufferSize;
    
    // apply window
    for(int n = 0; n < gFFTSize; n++) {
        timeDomainInL[n] = gInputBufferL[pointer] * gWindowBuffer[n];
        timeDomainInR[n] = gInputBufferR[pointer] * gWindowBuffer[n];
        pointer++;
        if(pointer >= gBufferSize)
            pointer = 0;
    }
    
    // do fft
    dsp_fft.perform(timeDomainInL, freqDomainInL, false);    // L FFT
    dsp_fft.perform(timeDomainInR, freqDomainInR, false);    // L FFT
    
    
    // do A to B
    // fftFreqL0a to fftFreqL0b
    int Beta = gBeta_; //gBeta;
    int positionChoice = positionChoice_; //gPositionChoice;
    int AZsubplaceWidth = AZsubplaceWidth_; // for tolerance of position setting
    int startPosition = positionChoice;
    int endPosition   = positionChoice;
//    float bandGain = 1; // apply on band EQ
    
    // calculate positionChoice range from AZ subplace width
//    startPosition = positionChoice - floor(AZsubplaceWidth/2);
//    endPosition   = positionChoice + floor(AZsubplaceWidth/2);
    if ( (AZsubplaceWidth % 2) == 1){
        startPosition = positionChoice - floor(AZsubplaceWidth/2);
        endPosition   = positionChoice + floor(AZsubplaceWidth/2);
    }
    else{
        if (positionChoice >= 0 ){
            startPosition = positionChoice - floor(AZsubplaceWidth/2);
            endPosition   = positionChoice + floor(AZsubplaceWidth/2) - 1;
        }
        else{
            startPosition = positionChoice - floor(AZsubplaceWidth/2) + 1;
            endPosition   = positionChoice + floor(AZsubplaceWidth/2);
        }
    }
    
    
    // clear bandXmax every time doing fft, for updating new value
    band01max = 0;
    band02max = 0;
    band03max = 0;
    band04max = 0;
    band05max = 0;
    band06max = 0;
    band07max = 0;
    band08max = 0;
    band09max = 0;
    band10max = 0;
    band11max = 0;
    
    // for the whole fft samples
    // maybe it can be reduced to only half of fft (positive)
    for ( int i=0; i < 8192; i++){
        
        // initialse parameter for each freq k bin
        float minAL = 10, maxAL = 0, minAR = 10, maxAR = 0;
        float AL=0, AR=0, magnitude = 0, magnitudeAfterGain = 0;
        int indexMinAL = 0, indexMinAR = 0;
        
        for (int iBeta = 1; iBeta <= Beta; iBeta++){
            
            // calculate Azimuth magnitude for both channel
            AL = abs( abs(freqDomainInL[i]) - float(iBeta) / float(Beta) * abs( freqDomainInR[i]));
            AR = abs( abs(freqDomainInR[i]) - float(iBeta) / float(Beta) * abs( freqDomainInL[i]));
            
            // save the min and max magnitude, and the corresponding index
            // L side
            if (AL < minAL){
                minAL = AL;
                indexMinAL = iBeta;
            } else if (AL > maxAL){
                maxAL = AL;
            }
            // R side
            if (AR < minAR){
                minAR = AR;
                indexMinAR = iBeta;
            } else if (AR > maxAR){
                maxAR = AR;
            }
        } // end for iBeta
        
        
        // positionChoice by User through UI slider
        for (positionChoice = startPosition; positionChoice <= endPosition ; positionChoice++){
            
            // judge if choose Right side, also the exact position
            // positionChoice >= Beta, "equal" is when choosing mid point
            if ( (positionChoice >= 0) && (minAR <= minAL) && ( Beta - positionChoice == indexMinAR)) {
                magnitude = maxAR - minAR;
            }
            // judge if choose Left side, also the exact position
            else if ( (positionChoice < 0) && ( Beta - abs(positionChoice) == indexMinAL) && (minAL <= minAR )) {
                magnitude = maxAL - minAL;
            }
            
            // calculate the magnitude, use the original phase (normalised)
            // L and R shoule be the same as the chosen position magnitude
            magnitudeAfterGain = bandGainCalculation(i, gFFTSize, magnitude);
            freqDomainOutL[i] = magnitudeAfterGain * freqDomainInL[i] / abs(freqDomainInL[i]); // L
            freqDomainOutR[i] = magnitudeAfterGain * freqDomainInR[i] / abs(freqDomainInR[i]); // R
        }   // end for positionChoice
        
    }   // end for i numSample
    
    // do ifft
    dsp_fft.perform(freqDomainOutL, timeDomainOutL, true);  // L IFFT
    dsp_fft.perform(freqDomainOutR, timeDomainOutR, true);  // R IFFT
    
    // Add timeDomainOut into the output buffer
    pointer = gOutputBufferWritePointer;
    for(int n = 0; n < gFFTSize; n++) {
        gOutputBufferL[pointer] += timeDomainOutL[n].real() * gWindowBuffer[n];
        gOutputBufferR[pointer] += timeDomainOutR[n].real() * gWindowBuffer[n];
        
        pointer++;
        if(pointer >= gBufferSize)
            pointer = 0;
    }

}


float sourceSeparationADRessAudioProcessor::bandGainCalculation(float binNum, int fftNumber, float magnitude)
{
    float binSize = sampleRate / fftNumber;
    if (binNum > fftNumber/2 )
        binNum = fftNumber - binNum;
    
    float newMag = 1;
    
    if (binNum * binSize <= 50.0){         newMag = magnitude * pow(10, band01Gain_ / 20); band01max = std::max(band01max, newMag);}
    else if (binNum * binSize <= 100.0){   newMag = magnitude * pow(10, band02Gain_ / 20); band02max = std::max(band02max, newMag);}
    else if (binNum * binSize <= 200.0){   newMag = magnitude * pow(10, band03Gain_ / 20); band03max = std::max(band03max, newMag);}
    else if (binNum * binSize <= 300.0){   newMag = magnitude * pow(10, band04Gain_ / 20); band04max = std::max(band04max, newMag);}
    else if (binNum * binSize <= 500.0){   newMag = magnitude * pow(10, band05Gain_ / 20); band05max = std::max(band05max, newMag);}
    else if (binNum * binSize <= 1000.0){  newMag = magnitude * pow(10, band06Gain_ / 20); band06max = std::max(band06max, newMag);}
    else if (binNum * binSize <= 2000.0){  newMag = magnitude * pow(10, band07Gain_ / 20); band07max = std::max(band07max, newMag);}
    else if (binNum * binSize <= 3000.0){  newMag = magnitude * pow(10, band08Gain_ / 20); band08max = std::max(band08max, newMag);}
    else if (binNum * binSize <= 5000.0){  newMag = magnitude * pow(10, band09Gain_ / 20); band09max = std::max(band09max, newMag);}
    else if (binNum * binSize <= 10000.0){ newMag = magnitude * pow(10, band10Gain_ / 20); band10max = std::max(band10max, newMag);}
    else{   /* normally, its <= 22050 */   newMag = magnitude * pow(10, band11Gain_ / 20); band11max = std::max(band11max, newMag);}
    
    return newMag;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new sourceSeparationADRessAudioProcessor();
}



