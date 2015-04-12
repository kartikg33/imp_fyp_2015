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

#include "PAWSBoard_UI.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
PAWSBoard_UI::PAWSBoard_UI ()
{
    addAndMakeVisible (comboBox = new ComboBox ("cb_selectPort"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centred);
    comboBox->setTextWhenNothingSelected (TRANS("Select Serial Port"));
    comboBox->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    comboBox->addListener (this);

    addAndMakeVisible (groupComponent = new GroupComponent ("group_Function",
                                                            TRANS("Select Function")));
    groupComponent->setTextLabelPosition (Justification::centred);
    groupComponent->setColour (GroupComponent::outlineColourId, Colour (0x66ffffff));
    groupComponent->setColour (GroupComponent::textColourId, Colours::white);

    addAndMakeVisible (toggleButton = new ToggleButton ("tb_Voice"));
    toggleButton->setButtonText (TRANS("Voice"));
    toggleButton->addListener (this);
    toggleButton->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (toggleButton2 = new ToggleButton ("tb_Sample"));
    toggleButton2->setButtonText (TRANS("Sample"));
    toggleButton2->addListener (this);
    toggleButton2->setColour (ToggleButton::textColourId, Colours::white);

    addAndMakeVisible (label = new Label ("lb_board",
                                          TRANS("Board")));
    label->setFont (Font (21.60f, Font::plain));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::backgroundColourId, Colour (0x00ffffff));
    label->setColour (Label::textColourId, Colours::white);
    label->setColour (Label::outlineColourId, Colour (0x00ffffff));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00d82929));
    label->setColour (TextEditor::highlightColourId, Colour (0x40ffffff));

    addAndMakeVisible (slider = new Slider ("volume"));
    slider->setRange (0, 1, 0);
    slider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    slider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
    slider->setColour (Slider::backgroundColourId, Colour (0xffdc1e1e));
    slider->setColour (Slider::thumbColourId, Colour (0xffcbd88e));
    slider->setColour (Slider::trackColourId, Colour (0xff0f3abc));
    slider->setColour (Slider::rotarySliderFillColourId, Colour (0xff0f3abc));
    slider->setColour (Slider::rotarySliderOutlineColourId, Colours::white);
    slider->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (185, 220);


    //[Constructor] You can add your own custom stuff here..
    slider->setValue(0.5);
    int i;
    for(i = 0; i< 3; i++){
        threads[i] = nullptr;
    }
    initBuffer();
    //pthread_create(&threads[2], NULL,initBuff,(void*)this);
    listSerial();
    toggleButton->setRadioGroupId(1,(NotificationType)0);
    toggleButton2->setRadioGroupId(1,(NotificationType)0);

    //[/Constructor]
}

PAWSBoard_UI::~PAWSBoard_UI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    alive = false;
    VoiceFl = false;
    SampleFl = false;
    closePort(serport);
    samplebuff->~AudioSampleBuffer();
    //[/Destructor_pre]

    comboBox = nullptr;
    groupComponent = nullptr;
    toggleButton = nullptr;
    toggleButton2 = nullptr;
    label = nullptr;
    slider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    int i;
    for(i = 0; i <3; i++){
        if(threads[i] != nullptr){
            pthread_cancel(threads[i]);
            threads[i] = nullptr;
        }
    }
    //[/Destructor]
}

//==============================================================================
void PAWSBoard_UI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setGradientFill (ColourGradient (Colours::black,
                                       88.0f, 80.0f,
                                       Colour (0xff311e1e),
                                       176.0f, 240.0f,
                                       true));
    g.fillRoundedRectangle (0.0f, 24.0f, 184.0f, 162.0f, 18.500f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void PAWSBoard_UI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBox->setBounds (94 - (171 / 2), 61 - (24 / 2), 171, 24);
    groupComponent->setBounds (8, 88, 168, 64);
    toggleButton->setBounds (24, 112, 72, 24);
    toggleButton2->setBounds (88, 112, 72, 24);
    label->setBounds (8, 8, 168, 32);
    slider->setBounds ((getWidth() / 2) - (80 / 2), 155, 80, 60);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void PAWSBoard_UI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..

        //set serport (-1 if not connected)
        connectPort((char*)static_cast<const char*>(comboBox->getText().toUTF8()));
        if(VoiceFl){
            buttonClicked(toggleButton);
        }else if(SampleFl){
            buttonClicked(toggleButton2);
        }


        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void PAWSBoard_UI::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == toggleButton)
    {
        //[UserButtonCode_toggleButton] -- add your button handler code here..

        VoiceFl = true;
        SampleFl = false;
        //std::cout<<(&threads[0])<<newLine;
        if(threads[0]!=nullptr)
            pthread_cancel(threads[0]);
        pthread_create(&threads[0], NULL,playVoice,(void*)this);

        //[/UserButtonCode_toggleButton]
    }
    else if (buttonThatWasClicked == toggleButton2)
    {
        //[UserButtonCode_toggleButton2] -- add your button handler code here..
        VoiceFl = false;
        SampleFl = true;

        //std::cout<<(&threads[1])<<newLine;
        if(threads[1]!=nullptr)
            pthread_cancel(threads[1]);
        pthread_create(&threads[1], NULL,playSample,(void*)this);

        //[/UserButtonCode_toggleButton2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void PAWSBoard_UI::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == slider)
    {
        //[UserSliderCode_slider] -- add your slider handling code here..
        amplitude = (float)slider->getValue();
        //std::cout<<amplitude<<newLine;
        //[/UserSliderCode_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void PAWSBoard_UI::listSerial(){

    comboBox->addItem(ard, 1);
    comboBox->addItem(blu,2);
    comboBox->addItem("NULL", 3);
}

void PAWSBoard_UI::connectPort(char* ser){
    struct termios options;
    memset(&options,0,sizeof(options));
    char *device = ser;

    closePort(serport);
    serport = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if(serport == -1) {
        std::cout<<"Failed to connect to Arduino\n";
        initBuffer();
    } else {
        std::cout<<"Connected to Arduino\n";
        tcgetattr(serport, &options);   // read serial port options
        //options.c_lflag |= ICANON; //CANONICAL MODE
        cfsetospeed(&options,B115200); // B115200 baud // B230400
        cfsetispeed(&options,B115200); // B115200 baud
        tcsetattr(serport,TCSANOW, &options);
        fcntl(serport, F_SETFL, 0);    // clear all flags on descriptor, enable direct I/O
    }
}

void PAWSBoard_UI::closePort(int fd){
    if(fd!=-1){
        close(fd);
        std::cout<<"Closing connection to Arduino\n";
    }
}

void PAWSBoard_UI::initBuffer(){
    int i;
    for(i = 0; i < bufflen; i++){
        buffL[i] = 0;
        buffR[i] = 0;
    }
}

void *initBuff(void * dummy){
    PAWSBoard_UI *obj = (PAWSBoard_UI *) dummy;
    float t = 29/512;
    while (obj->alive) {
        obj->buffL[obj->buffptr] = 0;
        obj->buffR[obj->buffptr] = 0;
        
        obj->buffptr++;
        if(obj->buffptr>=obj->bufflen)
            obj->buffptr = 0;

        sleep(t);
    }
    obj = nullptr;
    delete obj;
    return 0;
}

void *playVoice(void* dummy){
    std::cout<<"Voice Function"<<newLine;
    PAWSBoard_UI *obj = (PAWSBoard_UI *) dummy;
    static const int len = 30;
    float val;
    int i;
    char c;
    while(obj->VoiceFl && obj->serport!=-1){
        char ascii_int[len] = {0};
        c = NULL;
        i = 0;
        //std::cout<<"reading\n";
        read(obj->serport, &c, 1);
        //std::cout<<c<<newLine;
        while ((c != '\n')&&(i<len)){
            ascii_int[i++] = c;
            read(obj->serport, &c, 1);
            //std::cout<<c<<newLine;
        }
        //std::cout<<"read\n";
        val = (atof(ascii_int)-512)*0.00195f*obj->amplitude;
        obj->buffL[obj->buffptr] = val;
        obj->buffR[obj->buffptr] = val;

        
        obj->buffptr++;
        if(obj->buffptr>=obj->bufflen)
            obj->buffptr = 0;

        //std::cout<<obj->buffL[obj->buffptr]<<newLine;
    }
    obj = nullptr;
    delete obj;
    std::cout<<"End Voice Function"<<newLine;
    return 0;
}

void *playSample(void* dummy){
    std::cout<<"Sample Function"<<newLine;
    PAWSBoard_UI *obj = (PAWSBoard_UI *) dummy;
    obj->loadSample(drum);
    
    static const int len = 500;
    int tempbuff[len] = {0};
    int tempptr = 0;
    int diff = 0;
    int change = 0;
    int thresh = 100;
    
    bool play = false;
    pthread_t t = nullptr;
    /*
    pthread_t t;
    pthread_create(&t, NULL,initSamp,(void*)obj);
     */

    
    static const int dist = 30;
    int i;
    char c;
    
    while(obj->SampleFl && obj->serport!=-1){
        
        char ascii_int[dist] = {0};
        c = NULL;
        i = 0;
        read(obj->serport, &c, 1);
        while ((c != '\n')&&(i<dist)){
            ascii_int[i++] = c;
            read(obj->serport, &c, 1);
        }
        tempbuff[tempptr] = atoi(ascii_int);
        //std::cout<<tempptr<<" "<<tempbuff[tempptr]<<newLine;
        
        diff = tempptr+1;
        if (diff>=len)
            diff = 0;
        
        change = tempbuff[tempptr] - tempbuff[diff];
        change = abs(change);
        //std::cout<<tempbuff[tempptr]<<" - "<<tempbuff[diff]<<newLine;
        //std::cout<<change<<newLine;
        
        
        if (change<(thresh*0.8) && play==true){
            play = false;
        }
        if(change>=thresh && play==false){
            //std::cout<<change<<newLine;
            
            play = true;
            
            
            //t_active = true;
            if(t!=nullptr){
                pthread_cancel(t);
            }
            pthread_create(&t, NULL,addSamp,(void*)obj);
            
            
            /*
            obj->buffL[obj->buffptr] = 1;
            obj->buffR[obj->buffptr] = 1;
             */
            
            
        }
        
        tempptr++;
        if(tempptr>=len)
            tempptr = 0;
        
    }
    
    obj = nullptr;
    delete obj;
    std::cout<<"End Sample Function"<<newLine;
    return 0;
    
}

void *addSamp(void* dummy){
    PAWSBoard_UI *obj = (PAWSBoard_UI *) dummy;
    float tim = (float)1/5000;
    for(int i = 0; i < obj->samplebuff->getNumSamples(); i++){
        obj->buffL[obj->buffptr] = obj->samplebuff->getSample(0, i)*obj->amplitude;
        obj->buffR[obj->buffptr] = obj->samplebuff->getSample(1, i)*obj->amplitude;
        //std::cout<<obj->buffL[ptr]<<newLine;
        
        obj->buffptr++;
        if(obj->buffptr>=obj->bufflen)
            obj->buffptr = 0;
        sleep(tim);
    }
    obj = nullptr;
    delete obj;
    return 0;
}

void *initSamp(void*dummy){
    PAWSBoard_UI *obj = (PAWSBoard_UI *) dummy;
    obj->initBuffer();
    pthread_t t;
    sleep(1);
    pthread_create(&t, NULL,initSamp,(void*)obj);
    return 0;
}


void PAWSBoard_UI::loadSample(String samp){
    if(storedsamp != samp){
        File _file = File(samp);
        WavAudioFormat _wavAudioFormat; //= new WavAudioFormat();
        AudioFormatReader * _audioFormatReader = _wavAudioFormat.createReaderFor(_file.createInputStream(), 0);
        samplebuff = new AudioSampleBuffer();
        samplebuff->setSize(2, _audioFormatReader->lengthInSamples);
        _audioFormatReader->read(samplebuff, (int)0, _audioFormatReader->lengthInSamples, (int64)0, true, true);
        storedsamp = samp;
        //delete _wavAudioFormat;
        delete _audioFormatReader;
        
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="PAWSBoard_UI" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="185" initialHeight="220">
  <BACKGROUND backgroundColour="0">
    <ROUNDRECT pos="0 24 184 162" cornerSize="18.5" fill=" radial: 88 80, 176 240, 0=ff000000, 1=ff311e1e"
               hasStroke="0"/>
  </BACKGROUND>
  <COMBOBOX name="cb_selectPort" id="e2c2900c808bd7ce" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="93.5c 61c 171 24"
            editable="0" layout="36" items="" textWhenNonSelected="Select Serial Port"
            textWhenNoItems="(no choices)"/>
  <GROUPCOMPONENT name="group_Function" id="79d81df0746b0b06" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 88 168 64" outlinecol="66ffffff"
                  textcol="ffffffff" title="Select Function" textpos="36"/>
  <TOGGLEBUTTON name="tb_Voice" id="35f20342a4396c51" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="24 112 72 24" txtcol="ffffffff"
                buttonText="Voice" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="tb_Sample" id="e9bcf99bae131cfc" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="88 112 72 24" txtcol="ffffffff"
                buttonText="Sample" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <LABEL name="lb_board" id="ba5212fc8a6e23f4" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 168 32" bkgCol="ffffff" textCol="ffffffff"
         outlineCol="ffffff" edTextCol="ff000000" edBkgCol="d82929" hiliteCol="40ffffff"
         labelText="Board" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="21.600000000000001421"
         bold="0" italic="0" justification="36"/>
  <SLIDER name="volume" id="ab49a4a49402c5a0" memberName="slider" virtualName=""
          explicitFocusOrder="0" pos="0Cc 155 80 60" bkgcol="ffdc1e1e"
          thumbcol="ffcbd88e" trackcol="ff0f3abc" rotarysliderfill="ff0f3abc"
          rotaryslideroutline="ffffffff" min="0" max="1" int="0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="0" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
