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
#include <cstring>

#ifdef _MSC_VER
#define snprintf _snprintf_s //support for pre-2014 versions of Visual Studio
#endif // _MSC_VER

//==============================================================================
sourceSeparationADRessAudioProcessorEditor::sourceSeparationADRessAudioProcessorEditor (sourceSeparationADRessAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
        gBetaLabel_("", "Beta"),
        positionChoiceLabel_("", "Azimuth position"),
        AZsubplaceWidthLabel_("", "Azimuth subplace width"),
        lowGainLabel_("", "Gain (dB)"),

        band01GainLabel_("", "50"),
        band02GainLabel_("", "100"),
        band03GainLabel_("", "200"),
        band04GainLabel_("", "300"),
        band05GainLabel_("", "500"),
        band06GainLabel_("", "1k"),
        band07GainLabel_("", "2k"),
        band08GainLabel_("", "3k"),
        band09GainLabel_("", "5k"),
        band10GainLabel_("", "10k"),
        band11GainLabel_("", "20k")

//      bandwidthLabel_("", "Bandwidth:")
{
    // Set up the sliders

    addAndMakeVisible(&gBetaSlider_);
    gBetaSlider_.setSliderStyle(Slider::LinearHorizontal);
    gBetaSlider_.addListener(this);
    gBetaSlider_.setRange(2, 9, 1 );
//    gBetaSlider_.setRange(10.0, 20000.0, 0.1);

    // Make the centre frequency slider (approximately) logarithmic for a
    // more natural feel
//    gBetaSlider_.setSkewFactorFromMidPoint(sqrt(10.0 * 15000.0));

    /* add for compression */
    addAndMakeVisible(&positionChoiceSlider_);
    positionChoiceSlider_.setSliderStyle(Slider::ThreeValueHorizontal);
    positionChoiceSlider_.addListener(this);
    positionChoiceSlider_.setRange(-8, 8, 1);
    positionChoiceSlider_.setValue(0.0);
    positionChoiceSlider_.setMaxValue(6);
    positionChoiceSlider_.setMinValue(-6);

    addAndMakeVisible(&AZsubplaceWidthSlider_);
    AZsubplaceWidthSlider_.setSliderStyle(Slider::Rotary);
    AZsubplaceWidthSlider_.addListener(this);
    AZsubplaceWidthSlider_.setRange(1.0, 5, 1);
    AZsubplaceWidthSlider_.setValue(1);

    addAndMakeVisible(&lowGainSlider_);
    lowGainSlider_.setSliderStyle(Slider::Rotary);
    lowGainSlider_.addListener(this);
    lowGainSlider_.setRange(-20, 40.0, 1);
    lowGainSlider_.setValue(6);

    addAndMakeVisible(&band01GainSlider_);
    addAndMakeVisible(&band02GainSlider_);
    addAndMakeVisible(&band03GainSlider_);
    addAndMakeVisible(&band04GainSlider_);
    addAndMakeVisible(&band05GainSlider_);
    addAndMakeVisible(&band06GainSlider_);
    addAndMakeVisible(&band07GainSlider_);
    addAndMakeVisible(&band08GainSlider_);
    addAndMakeVisible(&band09GainSlider_);
    addAndMakeVisible(&band10GainSlider_);
    addAndMakeVisible(&band11GainSlider_);
    band01GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band02GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band03GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band04GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band05GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band06GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band07GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band08GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band09GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band10GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band11GainSlider_.setSliderStyle(Slider::LinearBarVertical);
    band01GainSlider_.addListener(this);
    band02GainSlider_.addListener(this);
    band03GainSlider_.addListener(this);
    band04GainSlider_.addListener(this);
    band05GainSlider_.addListener(this);
    band06GainSlider_.addListener(this);
    band07GainSlider_.addListener(this);
    band08GainSlider_.addListener(this);
    band09GainSlider_.addListener(this);
    band10GainSlider_.addListener(this);
    band11GainSlider_.addListener(this);
    band01GainSlider_.setRange(-60.0, 20.0, 1);
    band02GainSlider_.setRange(-60.0, 20.0, 1);
    band03GainSlider_.setRange(-60.0, 20.0, 1);
    band04GainSlider_.setRange(-60.0, 20.0, 1);
    band05GainSlider_.setRange(-60.0, 20.0, 1);
    band06GainSlider_.setRange(-60.0, 20.0, 1);
    band07GainSlider_.setRange(-60.0, 20.0, 1);
    band08GainSlider_.setRange(-60.0, 20.0, 1);
    band09GainSlider_.setRange(-60.0, 20.0, 1);
    band10GainSlider_.setRange(-60.0, 20.0, 1);
    band11GainSlider_.setRange(-60.0, 20.0, 1);
    band01GainSlider_.setValue(0);
    band02GainSlider_.setValue(0);
    band03GainSlider_.setValue(0);
    band04GainSlider_.setValue(0);
    band05GainSlider_.setValue(0);
    band06GainSlider_.setValue(0);
    band07GainSlider_.setValue(0);
    band08GainSlider_.setValue(0);
    band09GainSlider_.setValue(0);
    band10GainSlider_.setValue(0);
    band11GainSlider_.setValue(0);
    band01GainSlider_.setColour (Slider::trackColourId , Colours::antiquewhite);
    band02GainSlider_.setColour (Slider::trackColourId , Colours::aqua);
    band03GainSlider_.setColour (Slider::trackColourId , Colours::aquamarine);
    band04GainSlider_.setColour (Slider::trackColourId , Colours::azure);
    band05GainSlider_.setColour (Slider::trackColourId , Colours::cornsilk);
    band06GainSlider_.setColour (Slider::trackColourId , Colours::chartreuse);
    band07GainSlider_.setColour (Slider::trackColourId , Colours::deeppink);
    band08GainSlider_.setColour (Slider::trackColourId , Colours::coral);
    band09GainSlider_.setColour (Slider::trackColourId , Colours::dodgerblue);
    band10GainSlider_.setColour (Slider::trackColourId , Colours::burlywood);
    band11GainSlider_.setColour (Slider::trackColourId , Colours::cyan);



//    // This label is informational and exists apart from other controls
//    // The other labels are attached to sliders and combo boxes
//    addAndMakeVisible(&bandwidthLabel_);
//    bandwidthLabel_.setFont(Font (12.0f));

    gBetaLabel_.attachToComponent(&gBetaSlider_, false);
    gBetaLabel_.setFont(Font (11.0f));

    positionChoiceLabel_.attachToComponent(&positionChoiceSlider_, false);
    positionChoiceLabel_.setFont(Font (11.0f));

    AZsubplaceWidthLabel_.attachToComponent(&AZsubplaceWidthSlider_, false);
    AZsubplaceWidthLabel_.setFont(Font (11.0f));

    lowGainLabel_.attachToComponent(&lowGainSlider_, false);
    lowGainLabel_.setFont(Font (11.0f));

    band01GainLabel_.attachToComponent(&band01GainSlider_, false);
    band02GainLabel_.attachToComponent(&band02GainSlider_, false);
    band03GainLabel_.attachToComponent(&band03GainSlider_, false);
    band04GainLabel_.attachToComponent(&band04GainSlider_, false);
    band05GainLabel_.attachToComponent(&band05GainSlider_, false);
    band06GainLabel_.attachToComponent(&band06GainSlider_, false);
    band07GainLabel_.attachToComponent(&band07GainSlider_, false);
    band08GainLabel_.attachToComponent(&band08GainSlider_, false);
    band09GainLabel_.attachToComponent(&band09GainSlider_, false);
    band10GainLabel_.attachToComponent(&band10GainSlider_, false);
    band11GainLabel_.attachToComponent(&band11GainSlider_, false);
    band01GainLabel_.setFont(Font (11.0f));
    band02GainLabel_.setFont(Font (11.0f));
    band03GainLabel_.setFont(Font (11.0f));
    band04GainLabel_.setFont(Font (11.0f));
    band05GainLabel_.setFont(Font (11.0f));
    band06GainLabel_.setFont(Font (11.0f));
    band07GainLabel_.setFont(Font (11.0f));
    band08GainLabel_.setFont(Font (11.0f));
    band09GainLabel_.setFont(Font (11.0f));
    band10GainLabel_.setFont(Font (11.0f));
    band11GainLabel_.setFont(Font (11.0f));


    // add the triangular resizer component for the bottom-right of the UI
    addAndMakeVisible(resizer_ = new ResizableCornerComponent (this, &resizeLimits_));
    resizeLimits_.setSizeLimits(500, 500, 500, 500);

    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize(500, 500);
//    setSize(ownerFilter->lastUIWidth_,
//            ownerFilter->lastUIHeight_);

    startTimer(50);   // default
//    startTimer(500);    // reduce the computation loading

    
    barBand01.setColour (TextButton::buttonColourId, Colours::firebrick);
    barBand02.setColour (TextButton::buttonColourId, Colours::floralwhite);
    barBand03.setColour (TextButton::buttonColourId, Colours::forestgreen);
    barBand04.setColour (TextButton::buttonColourId, Colours::fuchsia);
    barBand05.setColour (TextButton::buttonColourId, Colours::gainsboro);
    barBand06.setColour (TextButton::buttonColourId, Colours::ghostwhite);
    barBand07.setColour (TextButton::buttonColourId, Colours::gold);
    barBand08.setColour (TextButton::buttonColourId, Colours::goldenrod);
    barBand09.setColour (TextButton::buttonColourId, Colours::greenyellow);
    barBand10.setColour (TextButton::buttonColourId, Colours::honeydew);
    barBand11.setColour (TextButton::buttonColourId, Colours::hotpink);
    
    addAndMakeVisible (barBand01);
    addAndMakeVisible (barBand02);
    addAndMakeVisible (barBand03);
    addAndMakeVisible (barBand04);
    addAndMakeVisible (barBand05);
    addAndMakeVisible (barBand06);
    addAndMakeVisible (barBand07);
    addAndMakeVisible (barBand08);
    addAndMakeVisible (barBand09);
    addAndMakeVisible (barBand10);
    addAndMakeVisible (barBand11);

}

sourceSeparationADRessAudioProcessorEditor::~sourceSeparationADRessAudioProcessorEditor()
{
}

//==============================================================================
void sourceSeparationADRessAudioProcessorEditor::paint (Graphics& g)
{
//    g.fillAll (Colours::darkseagreen);
    g.fillAll (Colours::lightpink);
    
    g.setOpacity(0.3);
//    g.fillRect(20, 200, 20, 60);
////    Parallelogram<void>( float topLeft = 20, float topRight = 300, float bottomLeft = 400);
//    Parallelogram<void>(  Point (float 20, float 200), static_cast<void>(static_cast<void>(30), 200), (static_cast<void>(20, 300)) );
}

void sourceSeparationADRessAudioProcessorEditor::resized()
{
    gBetaSlider_.setBounds          (20, 20, 300, 40);
    positionChoiceSlider_.setBounds (20, 100, 300, 40);

    AZsubplaceWidthSlider_.setBounds(320, 20, 150, 40);
    lowGainSlider_.setBounds        (320, 100, 150, 40);

    band01GainSlider_.setBounds(100 - 50, 300, 30, 150);
    band02GainSlider_.setBounds(130 - 50, 300, 30, 150);
    band03GainSlider_.setBounds(160 - 50, 300, 30, 150);
    band04GainSlider_.setBounds(190 - 50, 300, 30, 150);
    band05GainSlider_.setBounds(220 - 50, 300, 30, 150);
    band06GainSlider_.setBounds(250 - 50, 300, 30, 150);
    band07GainSlider_.setBounds(280 - 50, 300, 30, 150);
    band08GainSlider_.setBounds(310 - 50, 300, 30, 150);
    band09GainSlider_.setBounds(340 - 50, 300, 30, 150);
    band10GainSlider_.setBounds(370 - 50, 300, 30, 150);
    band11GainSlider_.setBounds(400 - 50, 300, 30, 150);

//    bandwidthLabel_.setBounds(20, 70, 350, 20);

    resizer_->setBounds(getWidth() - 16, getHeight() - 16, 16, 16);

    getProcessor()->lastUIWidth_ = getWidth();
    getProcessor()->lastUIHeight_ = getHeight();


}

//==============================================================================
// This timer periodically checks whether any of the filter's parameters have changed...
void sourceSeparationADRessAudioProcessorEditor::timerCallback()
{
    sourceSeparationADRessAudioProcessor* ourProcessor = getProcessor();

    gBetaSlider_.setValue(ourProcessor->gBeta_, dontSendNotification);
    positionChoiceSlider_.setValue(ourProcessor->positionChoice_, dontSendNotification);
    AZsubplaceWidthSlider_.setValue(ourProcessor->AZsubplaceWidth_, dontSendNotification);
    lowGainSlider_.setValue(ourProcessor->lowGain_, dontSendNotification);

    band01GainSlider_.setValue(ourProcessor->band01Gain_, dontSendNotification);
    band02GainSlider_.setValue(ourProcessor->band02Gain_, dontSendNotification);
    band03GainSlider_.setValue(ourProcessor->band03Gain_, dontSendNotification);
    band04GainSlider_.setValue(ourProcessor->band04Gain_, dontSendNotification);
    band05GainSlider_.setValue(ourProcessor->band05Gain_, dontSendNotification);
    band06GainSlider_.setValue(ourProcessor->band06Gain_, dontSendNotification);
    band07GainSlider_.setValue(ourProcessor->band07Gain_, dontSendNotification);
    band08GainSlider_.setValue(ourProcessor->band08Gain_, dontSendNotification);
    band09GainSlider_.setValue(ourProcessor->band09Gain_, dontSendNotification);
    band10GainSlider_.setValue(ourProcessor->band10Gain_, dontSendNotification);
    band11GainSlider_.setValue(ourProcessor->band11Gain_, dontSendNotification);


//    updateBandwidthLabel();
    
    float heightBand01 = 100 - (-20 * log10 ( ourProcessor -> band01max + 0.00001));
    float heightBand02 = 100 - (-20 * log10 ( ourProcessor -> band02max + 0.00001));
    float heightBand03 = 100 - (-20 * log10 ( ourProcessor -> band03max + 0.00001));
    float heightBand04 = 100 - (-20 * log10 ( ourProcessor -> band04max + 0.00001));
    float heightBand05 = 100 - (-20 * log10 ( ourProcessor -> band05max + 0.00001));
    float heightBand06 = 100 - (-20 * log10 ( ourProcessor -> band06max + 0.00001));
    float heightBand07 = 100 - (-20 * log10 ( ourProcessor -> band07max + 0.00001));
    float heightBand08 = 100 - (-20 * log10 ( ourProcessor -> band08max + 0.00001));
    float heightBand09 = 100 - (-20 * log10 ( ourProcessor -> band09max + 0.00001));
    float heightBand10 = 100 - (-20 * log10 ( ourProcessor -> band10max + 0.00001));
    float heightBand11 = 100 - (-20 * log10 ( ourProcessor -> band11max + 0.00001));
    

    barBand01.setBounds(27.5 + 30 *  1, 280 - heightBand01, 15, heightBand01);
    barBand02.setBounds(27.5 + 30 *  2, 280 - heightBand02, 15, heightBand02);
    barBand03.setBounds(27.5 + 30 *  3, 280 - heightBand03, 15, heightBand03);
    barBand04.setBounds(27.5 + 30 *  4, 280 - heightBand04, 15, heightBand04);
    barBand05.setBounds(27.5 + 30 *  5, 280 - heightBand05, 15, heightBand05);
    barBand06.setBounds(27.5 + 30 *  6, 280 - heightBand06, 15, heightBand06);
    barBand07.setBounds(27.5 + 30 *  7, 280 - heightBand07, 15, heightBand07);
    barBand08.setBounds(27.5 + 30 *  8, 280 - heightBand08, 15, heightBand08);
    barBand09.setBounds(27.5 + 30 *  9, 280 - heightBand09, 15, heightBand09);
    barBand10.setBounds(27.5 + 30 * 10, 280 - heightBand10, 15, heightBand10);
    barBand11.setBounds(27.5 + 30 * 11, 280 - heightBand11, 15, heightBand11);
}

// This is our Slider::Listener callback, when the user drags a slider.
void sourceSeparationADRessAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    // It's vital to use setParameterNotifyingHost to change any parameters that are automatable
    // by the host, rather than just modifying them directly, otherwise the host won't know
    // that they've changed.

    if (slider == &gBetaSlider_)
    {
        getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kgBetaParam,
                                                   (float)gBetaSlider_.getValue());
        updateBeta();
//        updateBandwidthLabel();
    }
    else if (slider == &positionChoiceSlider_)
    {
        getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kpositionChoiceParam,
                                                   (float)positionChoiceSlider_.getValue());
        updateBeta();
    }
    else if (slider == &AZsubplaceWidthSlider_)
    {
        getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kAZsubplaceWidthParam,
                                                   (float)AZsubplaceWidthSlider_.getValue());
    }
    else if (slider == &lowGainSlider_)
    {
        getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::klowGainParam,
                                                   (float)lowGainSlider_.getValue());
    }
    
    else if (slider == &band01GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband01GainParam,(float)band01GainSlider_.getValue());}
    else if (slider == &band02GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband02GainParam,(float)band02GainSlider_.getValue());}
    else if (slider == &band03GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband03GainParam,(float)band03GainSlider_.getValue());}
    else if (slider == &band04GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband04GainParam,(float)band04GainSlider_.getValue());}
    else if (slider == &band05GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband05GainParam,(float)band05GainSlider_.getValue());}
    else if (slider == &band06GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband06GainParam,(float)band06GainSlider_.getValue());}
    else if (slider == &band07GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband07GainParam,(float)band07GainSlider_.getValue());}
    else if (slider == &band08GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband08GainParam,(float)band08GainSlider_.getValue());}
    else if (slider == &band09GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband09GainParam,(float)band09GainSlider_.getValue());}
    else if (slider == &band10GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband10GainParam,(float)band10GainSlider_.getValue());}
    else if (slider == &band11GainSlider_){getProcessor()->setParameterNotifyingHost (sourceSeparationADRessAudioProcessor::kband11GainParam,(float)band11GainSlider_.getValue());}



}


void sourceSeparationADRessAudioProcessorEditor::updateBeta()
{
    sourceSeparationADRessAudioProcessor* ourProcessor = getProcessor();
    float maxValue = ourProcessor -> gBeta_ - 1 ;
    float minValue = ourProcessor -> gBeta_ *(-1) + 1;
    positionChoiceSlider_.setMaxValue(maxValue);
    positionChoiceSlider_.setMinValue(minValue);

//    limeContent.setSize(20, ourProcessor -> gBeta_ * 10);

}
