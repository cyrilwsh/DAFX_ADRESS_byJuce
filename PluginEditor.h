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

#ifndef __PLUGINEDITOR_H_6E48F605__
#define __PLUGINEDITOR_H_6E48F605__

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================

class sourceSeparationADRessAudioProcessorEditor  : public AudioProcessorEditor,
                                    public Slider::Listener,
                                   public Timer
{
public:
    sourceSeparationADRessAudioProcessorEditor (sourceSeparationADRessAudioProcessor* ownerFilter);
    ~sourceSeparationADRessAudioProcessorEditor();

    //==============================================================================
    // This is just a standard Juce paint method...
    void timerCallback();
    void paint (Graphics& g);
    void resized();
    void sliderValueChanged (Slider*);
    
private:
    void updateBeta();
    
    Label gBetaLabel_; 
    

    Label positionChoiceLabel_;
    Label AZsubplaceWidthLabel_;
    Label lowGainLabel_;
    
    
    Label band01GainLabel_;
    Label band02GainLabel_;
    Label band03GainLabel_;
    Label band04GainLabel_;
    Label band05GainLabel_;
    Label band06GainLabel_;
    Label band07GainLabel_;
    Label band08GainLabel_;
    Label band09GainLabel_;
    Label band10GainLabel_;
    Label band11GainLabel_;
    
    
//    TextButton limeContent;
    TextButton barBand01;
    TextButton barBand02;
    TextButton barBand03;
    TextButton barBand04;
    TextButton barBand05;
    TextButton barBand06;
    TextButton barBand07;
    TextButton barBand08;
    TextButton barBand09;
    TextButton barBand10;
    TextButton barBand11;
    
    
    
    
//    Label bandwidthLabel_;
    
    
    Slider gBetaSlider_;
    
    
    Slider positionChoiceSlider_;
    Slider AZsubplaceWidthSlider_;
    Slider lowGainSlider_;

    
    Slider band01GainSlider_;
    Slider band02GainSlider_;
    Slider band03GainSlider_;
    Slider band04GainSlider_;
    Slider band05GainSlider_;
    Slider band06GainSlider_;
    Slider band07GainSlider_;
    Slider band08GainSlider_;
    Slider band09GainSlider_;
    Slider band10GainSlider_;
    Slider band11GainSlider_;

    
    
    ScopedPointer<ResizableCornerComponent> resizer_;
    ComponentBoundsConstrainer resizeLimits_;
    
    sourceSeparationADRessAudioProcessor* getProcessor() const
    {
        return static_cast <sourceSeparationADRessAudioProcessor*> (getAudioProcessor());
    }
};


#endif  // __PLUGINEDITOR_H_6E48F605__
