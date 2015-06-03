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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MainUI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
MainUI::MainUI ()
{
    addAndMakeVisible (textButton = new TextButton ("bt_Connect"));
    textButton->setButtonText (TRANS("CONNECT"));
    textButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    textButton->addListener (this);
    textButton->setColour (TextButton::buttonColourId, Colour (0xff311e1e));
    textButton->setColour (TextButton::buttonOnColourId, Colours::black);
    textButton->setColour (TextButton::textColourOnId, Colours::white);
    textButton->setColour (TextButton::textColourOffId, Colours::white);

    addAndMakeVisible (textButton2 = new TextButton ("bt_Disconnect"));
    textButton2->setButtonText (TRANS("DISCONNECT"));
    textButton2->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    textButton2->addListener (this);
    textButton2->setColour (TextButton::buttonColourId, Colour (0xff311e1e));
    textButton2->setColour (TextButton::buttonOnColourId, Colours::black);
    textButton2->setColour (TextButton::textColourOnId, Colours::white);
    textButton2->setColour (TextButton::textColourOffId, Colours::white);

    addAndMakeVisible (label = new Label ("title_PAWS",
                                          TRANS("PAWS")));
    label->setFont (Font (49.10f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colours::white);
    label->setColour (Label::outlineColourId, Colour (0x00b71616));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    label->setColour (TextEditor::highlightColourId, Colour (0x404949a8));

    addAndMakeVisible (label2 = new Label ("title_KG",
                                           TRANS("by Kartik Gohil")));
    label2->setFont (Font (18.20f, Font::plain));
    label2->setJustificationType (Justification::centred);
    label2->setEditable (false, false, false);
    label2->setColour (Label::textColourId, Colours::white);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (slider = new Slider ("new slider"));
    slider->setRange (0, 3, 0);
    slider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider->setColour (Slider::thumbColourId, Colour (0xffbc0f0f));
    slider->setColour (Slider::trackColourId, Colour (0xffbc0f0f));
    slider->setColour (Slider::rotarySliderFillColourId, Colour (0xffbc0f0f));
    slider->setColour (Slider::rotarySliderOutlineColourId, Colours::white);
    slider->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (800, 600);


    //[Constructor] You can add your own custom stuff here..
    textButton2->setVisible(0);
    slider->setVisible(0);
    slider->setValue(0.3);
    for(int i = 0; i<maxBoards;i++){
        boardUI[i] = nullptr;
    }
    queue_t = nullptr;
    //[/Constructor]
}

MainUI::~MainUI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    pthread_cancel(queue_t);
    //[/Destructor_pre]

    textButton = nullptr;
    textButton2 = nullptr;
    label = nullptr;
    label2 = nullptr;
    slider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..

    for(int i = 0; i<maxBoards;i++){
        boardUI[i] = nullptr;
    }



    //[/Destructor]
}

//==============================================================================
void MainUI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MainUI::resized()
{
    //[UserPreResize] Add your own custom resize code here..

    int x,y;

    //[/UserPreResize]

    textButton->setBounds ((getWidth() / 2) + -75, (getHeight() / 2) + -12, 150, 24);
    textButton2->setBounds ((getWidth() / 2) + -80, getHeight() - 536, 150, 24);
    label->setBounds (16, 8, 120, 56);
    label2->setBounds (getWidth() - 128, getHeight() - 24, 126, 24);
    slider->setBounds ((getWidth() / 2) - (100 / 2), getHeight() - 140 - (80 / 2), 100, 80);
    //[UserResized] Add your own custom resize handling here..

    for(int i=0;i<numBoards;i++){
        x = boardUI[i]->getWidth();
        y = boardUI[i]->getHeight();
        boardUI[i]->setBounds(( getWidth()*(i+1)/(numBoards+1) - x/2 ) , ( getHeight()/2 - y/2 ) ,x,y);
    }

    //[/UserResized]
}

void MainUI::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton)
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        // CONNECT TO ARDUINO
        connectPort((char*)ard);
        if(serport != -1){
            for(int x = 0; x < maxBoards; x++){
                boardUI[x] = new Board();
                switch(x){
                    case 0: boardUI[x]->changeName("One"); break;
                    case 1: boardUI[x]->changeName("Two"); break;
                    case 2: boardUI[x]->changeName("Three"); break;
                }
                addAndMakeVisible(boardUI[x]);
            }
            numBoards = 3;
            slider->setVisible(1);
            textButton2->setVisible(1);
            resized();
            pthread_create(&queue_t, NULL,queueInput,(void*)this);
        }
        
        //[/UserButtonCode_textButton]s
    }
    else if (buttonThatWasClicked == textButton2)
    {
        //[UserButtonCode_textButton2] -- add your button handler code here..
        pthread_cancel(queue_t);
        while(numBoards>0){
            numBoards--;
            removeChildComponent(boardUI[numBoards]);
            boardUI[numBoards] = nullptr;
        }
        serport = closePort(serport);
        textButton2->setVisible(0);
        slider->setVisible(0);
        textButton->setVisible(1);
        //[/UserButtonCode_textButton2]
    }

    //[UserbuttonClicked_Post]
    
    //[/UserbuttonClicked_Post]
}

void MainUI::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        amplitude = (float)slider->getValue();
        //[/UserSliderCode_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

void MainUI::connectPort(char* ser){
    struct termios options;
    memset(&options,0,sizeof(options));
    char *device = ser;
    
    serport = closePort(serport);
    serport = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if(serport == -1) {
        std::cout<<"Failed to connect to Arduino\n";
    } else {
        std::cout<<"Connected to Arduino\n";
        tcgetattr(serport, &options);   // read serial port options
        //options.c_lflag |= ICANON; //CANONICAL MODE
        cfsetospeed(&options,B230400); // B115200 baud // B230400
        cfsetispeed(&options,B230400); // B115200 baud
        tcsetattr(serport,TCSANOW, &options);
        fcntl(serport, F_SETFL, 0);    // clear all flags on descriptor, enable direct I/O
    }
}

int MainUI::closePort(int fd){
    if(fd!=-1){
        close(fd);
        std::cout<<"Closing connection to Arduino\n";
    }
    return -1;
}


void *queueInput(void* dummy){
    
    std::cout<<"Queue Input"<<newLine;
    MainUI *obj = (MainUI *) dummy;
    //static const int len = 30;
    
    
    //coeffs.makeLowPass(6300.0, 3100.0);
    //IIRCoefficients * coeffs = new IIRCoefficients(0.0345944930030068,	0.0455180742182364,	0.0626818320117175,	0.0728764050076197,	0.0728764050076197, 0.0626818320117175);
    
    //IIRCoefficients * coeffs = new IIRCoefficients(double(0.0004116),double(0.0007137),double(0.0012791),double(0.0020807),double(0.0031537),double(0.0045223));
    
    
        /*
     IIRCoefficients * coeffs = new IIRCoefficients(0.122831334667863,
     0.0470393871492429,
     0.0506336736466862,
     0.0506336736466862,
     0.0470393871492429,
     0.122831334667863);
        */
    /*
    IIRCoefficients newcoeffs = *coeffs;
    for(int x = 0; x <= 5; x++){
        std::cout<<newcoeffs.coefficients[x]<<newLine;
    }
    */
    /*
     for(int x = 0; x < 5; x++){
     std::cout<<coeffs.coefficients[x]<<newLine;
     }
     */
    
    //obj->filt->setCoefficients(*coeffs);
    //IIRCoefficients stuff = obj->filt->getCoefficients();
    
    
    //std::cout<< obj->filt->getCoefficients()<< newLine;
    int num = 2;
    float temp[num+1];
    int tempptr = 0;
    float *input = new float[num];
    char c;
    
    
    while(obj->serport!=-1){
        
        tempptr = 0;
        do{
            read(obj->serport, &c, 1);
            temp[tempptr] = Byte(c)*4.0f;//((float(Byte(c))*4.0f)-512)*0.00195f*obj->amplitude;
            ++tempptr;
        } while (c!='\n');
        
        
        for(int x = 0; x < num; x++){
            read(obj->serport, &c, 1);
            input[x] = Byte(c)*4.0f;//((float(Byte(c))*4.0f)-512)*0.00195f*obj->amplitude;
        }
        
        // FILTERING HERE
        
        //obj->filt->processSamples(input, 10);
        
        // END FILTERING HERE
        int i = num+1-tempptr;
        for(int x = 0; x < tempptr-1; x++){
            obj->boardUI[i]->queuewrite++;
            if(obj->boardUI[i]->queuewrite>=obj->boardUI[i]->bufflen){
                obj->boardUI[i]->queuewrite = 0;
            }
            //std::cout<<input<<newLine;
            obj->boardUI[i]->queue[obj->boardUI[i]->queuewrite] = temp[x];
            i++;
            
        }
        
        for(int x = 0; x < num; x++){
            obj->boardUI[x]->queuewrite++;
            if(obj->boardUI[x]->queuewrite>=obj->boardUI[x]->bufflen){
                obj->boardUI[x]->queuewrite = 0;
            }
            //std::cout<<input[x]<<newLine;
            obj->boardUI[x]->queue[obj->boardUI[x]->queuewrite] = input[x];
        }
        
        
    }
    std::cout<<"End Queue Input"<<newLine;
    
    return 0;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MainUI" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="800" initialHeight="600">
  <BACKGROUND backgroundColour="ffffff"/>
  <TEXTBUTTON name="bt_Connect" id="7ca5f026f03eb2e0" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="-75C -12C 150 24"
              bgColOff="ff311e1e" bgColOn="ff000000" textCol="ffffffff" textColOn="ffffffff"
              buttonText="CONNECT" connectedEdges="15" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="bt_Disconnect" id="4f8034ba0a97717c" memberName="textButton2"
              virtualName="" explicitFocusOrder="0" pos="-80C 536R 150 24"
              bgColOff="ff311e1e" bgColOn="ff000000" textCol="ffffffff" textColOn="ffffffff"
              buttonText="DISCONNECT" connectedEdges="15" needsCallback="1"
              radioGroupId="0"/>
  <LABEL name="title_PAWS" id="a67ff662710583a9" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="16 8 120 56" textCol="ffffffff" outlineCol="b71616"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="404949a8" labelText="PAWS"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="49.100000000000001421" bold="0"
         italic="0" justification="36"/>
  <LABEL name="title_KG" id="dfaaf1c8ec4d098e" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="128R 24R 126 24" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="by Kartik Gohil"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="18.199999999999999289" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="new slider" id="8d0e9a8f2b8cefc0" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="0Cc 140Rc 100 80" thumbcol="ffbc0f0f"
          trackcol="ffbc0f0f" rotarysliderfill="ffbc0f0f" rotaryslideroutline="ffffffff"
          min="0" max="3" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
