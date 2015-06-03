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

#include "Board.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
Board::Board ()
{
    addAndMakeVisible (comboBox = new ComboBox ("cb_selectSamp"));
    comboBox->setEditableText (false);
    comboBox->setJustificationType (Justification::centred);
    comboBox->setTextWhenNothingSelected (TRANS("Select Sample File"));
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
    //label->setText(TRANS("one"), dontSendNotification);
    comboBox->setVisible(0);
    slider->setValue(0.5);
    int i;
    for(i = 0; i< 2; i++){
        threads[i] = nullptr;
    }
    initBuffer();
    listSamples();
    toggleButton->setRadioGroupId(1,(NotificationType)0);
    toggleButton2->setRadioGroupId(1,(NotificationType)0);

    samplebuff = new AudioSampleBuffer();
    samplebuff->setSize(2, 512);
    samplebuff->clear();

    //Generate Filter
    IIRCoefficients coeffs;
    coeffs.makePeakFilter(6300, 200, 1, 5);
    filt = new IIRFilter;
    filt->setCoefficients(coeffs);

    //[/Constructor]
}

Board::~Board()
{
    //[Destructor_pre]. You can add your own custom destruction code here..

    int i;
    for(i = 0; i <2; i++){
        if(threads[i] != nullptr){
            pthread_cancel(threads[i]);
            threads[i] = nullptr;
        }
    }

    VoiceFl = false;
    SampleFl = false;
    samplebuff = nullptr;

    //[/Destructor_pre]

    comboBox = nullptr;
    groupComponent = nullptr;
    toggleButton = nullptr;
    toggleButton2 = nullptr;
    label = nullptr;
    slider = nullptr;


    //[Destructor]. You can add your own custom destruction code here..


    filt = nullptr;
    //[/Destructor]
}

//==============================================================================
void Board::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.setGradientFill (ColourGradient (Colours::black,
                                       static_cast<float> ((getWidth() / 2) + -4), static_cast<float> ((getHeight() / 2) + -30),
                                       Colour (0xff311e1e),
                                       static_cast<float> ((getWidth() / 2) + 84), static_cast<float> ((getHeight() / 2) + 130),
                                       true));
    g.fillRoundedRectangle (0.0f, 24.0f, 184.0f, 162.0f, 18.500f);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Board::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    comboBox->setBounds (94 - (171 / 2), 130 - (24 / 2), 171, 24);
    groupComponent->setBounds (8, 40, 168, 64);
    toggleButton->setBounds (24, 64, 72, 24);
    toggleButton2->setBounds (88, 64, 72, 24);
    label->setBounds (8, 8, 168, 32);
    slider->setBounds ((getWidth() / 2) - (80 / 2), 155, 80, 60);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Board::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == comboBox)
    {
        //[UserComboBoxCode_comboBox] -- add your combo box handling code here..
        loadSample((char*)static_cast<const char*>(comboBox->getText().toUTF8()));
        //[/UserComboBoxCode_comboBox]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}

void Board::buttonClicked (Button* buttonThatWasClicked)
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
        comboBox->setVisible(0);
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
        
        loadSample("afrimid");
        comboBox->setSelectedItemIndex(1);
        comboBox->setVisible(1);
        //[/UserButtonCode_toggleButton2]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void Board::sliderValueChanged (Slider* sliderThatWasMoved)
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

void Board::listSamples(){
    
    comboBox->addItem("afrilo", 1);
    comboBox->addItem("afrimid", 2);
    comboBox->addItem("afrihi", 3);
}


void Board::initBuffer(){
    int i;
    for(i = 0; i < bufflen; i++){
        buffL[i] = 0;
        buffR[i] = 0;
    }
}


void *playVoice(void* dummy){
    std::cout<<"Voice Function"<<newLine;
    Board *obj = (Board *) dummy;


    float val, prev, delta;
    int inter = 7;
    prev = obj->buffL[obj->buffptr];

    while(obj->VoiceFl){

        while(obj->queueread != obj->queuewrite){ //POSSIBLE BREAKING POINT
            val = (obj->queue[obj->queueread]-512)*0.00195f*obj->amplitude;
            //val = obj->queue[obj->queueread];
            delta = float((val - prev)/inter);

            for(int x = 1; x < inter; x++){
                obj->buffptr++;
                if(obj->buffptr>=obj->bufflen)
                    obj->buffptr = 0;

                obj->buffL[obj->buffptr] = prev + float(x*delta);
                obj->buffR[obj->buffptr] = obj->buffL[obj->buffptr];
            }

            obj->queueread++;
            if(obj->queueread>=obj->bufflen)
                obj->queueread = 0;

            obj->buffptr++;
            if(obj->buffptr>=obj->bufflen)
                obj->buffptr = 0;

            obj->buffL[obj->buffptr] = val;
            obj->buffR[obj->buffptr] = val;

            prev = val;
        }
    }

    std::cout<<"End Voice Function"<<newLine;
    return 0;
}

void *playVoice_basic(void* dummy){
    std::cout<<"Voice Function"<<newLine;
    Board *obj = (Board *) dummy;

    float *chunk;
    float val, prev, delta;
    int inter = 7;
    val = obj->buffL[obj->buffptr];
    int length = 0;
    int diff = 0;

    while(obj->VoiceFl){

        //FIND CHUNK LENGTH
        if((diff = (obj->queuewrite - obj->queueread)%obj->bufflen) > 10){
            diff = 10;
        }
        length = diff*inter;
        chunk = new float[length];

        //INTERPOLATE CHUNK
        for(int x = 1; x <= diff; x++){
            prev = val;
            //val = (obj->queue[obj->queueread]-512)*0.00195f*obj->amplitude;
            val = obj->queue[obj->queueread];
            chunk[(x*7)-1] = val;

            obj->queueread++;
            if(obj->queueread>=obj->bufflen)
                obj->queueread = 0;

            delta = float((val - prev)/inter);

            for(int i = 1; x < inter; x++){
                chunk[((x-1)*7)+(i-1)] = prev + float(i*delta);
            }

        }

        //FILTER CHUNK



        //PUSH CHUNK TO BUFFER
        for(int x = 0; x<length; x++){

            obj->buffptr++;
            if(obj->buffptr>=obj->bufflen)
                obj->buffptr = 0;

            obj->buffL[obj->buffptr] = chunk[x];
            obj->buffR[obj->buffptr] = chunk[x];

        }

        delete[] chunk;
        chunk = nullptr;


    } //while(obj->VoiceFl){


    std::cout<<"End Voice Function"<<newLine;
    return 0;
}



void *playSample(void* dummy){
    std::cout<<"Sample Function"<<newLine;
    Board *obj = (Board *) dummy;
    static const int len = 50;
    int tempbuff[len] = {512};
    int tempptr = 0;
    int diff = 0;
    int change = 0;
    int thresh = 140;

    obj->playSamp = false;

    while(obj->SampleFl){

        while(obj->queueread != obj->queuewrite){
        tempbuff[tempptr] = obj->queue[obj->queueread];
        //std::cout<<tempptr<<" "<<tempbuff[tempptr]<<newLine;

        diff = tempptr+1;
        if (diff>=len)
            diff = 0;

        change = tempbuff[tempptr] - tempbuff[diff];
        change = abs(change);
        //std::cout<<tempbuff[tempptr]<<" - "<<tempbuff[diff]<<newLine;
        //std::cout<<change<<newLine;


        if (change<(thresh*0.8) && obj->playSamp==true){
            obj->playSamp = false;
        }
        if(change>=thresh && obj->playSamp==false){
            //std::cout<<change<<newLine;
            obj->playSamp = true;
        }

        //std::cout<<obj->playSamp<<newLine;

        ++tempptr;
        if(tempptr>=len)
            tempptr = 0;

        obj->queueread++;
        if(obj->queueread>=obj->bufflen)
            obj->queueread = 0;
        }

    }

    std::cout<<"End Sample Function"<<newLine;
    return 0;

}


void Board::loadSample(String samp){
    
    samp = samppath + samp + ".wav";
    
    if(storedsamp != samp){
        File _file = File(samp);
        WavAudioFormat _wavAudioFormat;
        ScopedPointer<AudioFormatReader> _audioFormatReader = _wavAudioFormat.createReaderFor(_file.createInputStream(), 0);

        samplebuff->clear();
        samplebuff = nullptr;
        samplebuff = new AudioSampleBuffer();

        samplebuff->setSize(2, _audioFormatReader->lengthInSamples);
        _audioFormatReader->read(samplebuff, (int)0, _audioFormatReader->lengthInSamples, (int64)0, true, true);
        storedsamp = samp;
        _audioFormatReader = nullptr;
    }
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Board" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="185"
                 initialHeight="220">
  <BACKGROUND backgroundColour="0">
    <ROUNDRECT pos="0 24 184 162" cornerSize="18.5" fill=" radial: -4C -30C, 84C 130C, 0=ff000000, 1=ff311e1e"
               hasStroke="0"/>
  </BACKGROUND>
  <COMBOBOX name="cb_selectSamp" id="e2c2900c808bd7ce" memberName="comboBox"
            virtualName="" explicitFocusOrder="0" pos="93.5c 130c 171 24"
            editable="0" layout="36" items="" textWhenNonSelected="Select Sample File"
            textWhenNoItems="(no choices)"/>
  <GROUPCOMPONENT name="group_Function" id="79d81df0746b0b06" memberName="groupComponent"
                  virtualName="" explicitFocusOrder="0" pos="8 40 168 64" outlinecol="66ffffff"
                  textcol="ffffffff" title="Select Function" textpos="36"/>
  <TOGGLEBUTTON name="tb_Voice" id="35f20342a4396c51" memberName="toggleButton"
                virtualName="" explicitFocusOrder="0" pos="24 64 72 24" txtcol="ffffffff"
                buttonText="Voice" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
  <TOGGLEBUTTON name="tb_Sample" id="e9bcf99bae131cfc" memberName="toggleButton2"
                virtualName="" explicitFocusOrder="0" pos="88 64 72 24" txtcol="ffffffff"
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
