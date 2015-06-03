/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_A3003833A5CF4C38__
#define __JUCE_HEADER_A3003833A5CF4C38__

//[Headers]     -- You can add your own extra header files here --
//#include "JuceHeader.h"
#include "../JuceLibraryCode/JuceHeader.h"

#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
//#include <thread>
//#include <chrono>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Board  : public Component,
               public ComboBoxListener,
               public ButtonListener,
               public SliderListener
{
public:
    //==============================================================================
    Board ();
    ~Board();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    static const int bufflen = 2000;
    float buffL[bufflen];
    float buffR[bufflen];
    float queue[bufflen];
    int buffptr = 0;
    int queueread = 0;
    int queuewrite = 0;
    bool VoiceFl = false;
    bool SampleFl = false;
    float amplitude = 0.0f;
    ScopedPointer<AudioSampleBuffer> samplebuff = nullptr;
    int sampBuffPtr = 0;
    String storedsamp = "NULL";
    ScopedPointer<IIRFilter> filt = nullptr;
    bool playSamp = false;
    bool sampPlaying = false;

    void initBuffer();
    void listSamples();
    void loadSample(String);

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    pthread_t threads[2];
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> comboBox;
    ScopedPointer<GroupComponent> groupComponent;
    ScopedPointer<ToggleButton> toggleButton;
    ScopedPointer<ToggleButton> toggleButton2;
    ScopedPointer<Label> label;
    ScopedPointer<Slider> slider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Board)
};

//[EndFile] You can add extra defines here...
#define drum "/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav"
void *playVoice_basic(void*);
void *playVoice(void*);
void *playSample(void*);
//[/EndFile]

#endif   // __JUCE_HEADER_A3003833A5CF4C38__
