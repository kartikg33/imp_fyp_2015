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

#ifndef __JUCE_HEADER_11346C81341AFB38__
#define __JUCE_HEADER_11346C81341AFB38__

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
class PAWSBoard_UI  : public Component,
                      public ComboBoxListener,
                      public ButtonListener,
                      public SliderListener
{
public:
    //==============================================================================
    PAWSBoard_UI ();
    ~PAWSBoard_UI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    static const int bufflen = 2000;
    float buffL[bufflen];
    float buffR[bufflen];
    int serport = -1;
    bool VoiceFl = false;
    bool SampleFl = false;
    int buffptr = 0;
    float amplitude = 0.0f;
    ScopedPointer<AudioSampleBuffer> samplebuff = nullptr;
    int sampBuffPtr = 0;
    String storedsamp = "NULL";
    bool alive = true;
    bool playSamp = false;
    bool sampPlaying = false;

    void initBuffer();
    void listSerial();
    void connectPort(char*);
    void closePort(int);
    void loadSample(String);

    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged);
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    pthread_t threads[3];
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ComboBox> comboBox;
    ScopedPointer<GroupComponent> groupComponent;
    ScopedPointer<ToggleButton> toggleButton;
    ScopedPointer<ToggleButton> toggleButton2;
    ScopedPointer<Label> label;
    ScopedPointer<Slider> slider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PAWSBoard_UI)
};

//[EndFile] You can add extra defines here...
#define ard "/dev/tty.usbmodem1d1121"
#define blu "/dev/tty.HC-06-DevB"
#define drum "/Users/kartikgohil/Documents/Imperial/Year4/Project/Test Audio/afrimid.wav"
void *playVoice(void*);
void *playVoice2(void*);
void *playSample(void*);
void *initSamp(void*);
void *addSamp(void*);
void *initBuff(void*);
//[/EndFile]

#endif   // __JUCE_HEADER_11346C81341AFB38__
