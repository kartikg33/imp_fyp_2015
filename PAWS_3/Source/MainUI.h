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

#ifndef __JUCE_HEADER_DD002BB6E5B99A9E__
#define __JUCE_HEADER_DD002BB6E5B99A9E__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"

#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MainUI  : public Component,
                public ButtonListener,
                public SliderListener
{
public:
    //==============================================================================
    MainUI ();
    ~MainUI();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    int numBoards = 0;
    static const int maxBoards = 3;
    float amplitude = 0.0f;
    ScopedPointer<Board> boardUI[maxBoards];
    int serport = -1;
    
    void connectPort(char*);
    int closePort(int);
    
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    int posx;
    int posy;
    pthread_t queue_t;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> textButton;
    ScopedPointer<TextButton> textButton2;
    ScopedPointer<Label> label;
    ScopedPointer<Label> label2;
    ScopedPointer<Slider> slider;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainUI)
};

//[EndFile] You can add extra defines here...
#define ard "/dev/tty.usbmodem1d1121"
void *queueInput(void*);
//[/EndFile]

#endif   // __JUCE_HEADER_DD002BB6E5B99A9E__
