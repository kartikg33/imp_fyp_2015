/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PAWSBoard_UI.cpp"
#include "AddBoardButton.cpp"
#include "Bluetooth.cpp"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class MainContentComponent   : public AudioAppComponent, private Timer
{
public:
    //==============================================================================
    MainContentComponent()
    :
        waveformL(),
        waveformR(),
        waveptr(0),
        channelDataL(),
        channelDataR(),
        sampleRate(0.0),
        expectedSamplesPerBlock (0)
    
    {
        setSize (800, 600);
        setOpaque(1);
        

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        //setSampleRate ();
        
        addAndMakeVisible (overlay = new AddBoardButton());
        //startTimerHz (2*sampleRate/(wavelen)); // use a timer to keep repainting this component
        startTimerHz (75); // use a timer to keep repainting this component

    }

    ~MainContentComponent()
    {
        
        delete overlay;
        overlay = nullptr;
        delete[] waveformL;
        waveformL = nullptr;
        delete[] waveformR;
        waveformR = nullptr;
        
        shutdownAudio();
        
    }

    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double newSampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()

        sampleRate = newSampleRate;
        expectedSamplesPerBlock = samplesPerBlockExpected;
        
        waveformL = new float[wavelen];
        waveformR = new float[wavelen];
        for(int x = 0; x<wavelen;x++){
            waveformL[x] = 0;
            waveformR[x] = 0;
        }
        
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!
        // For more details, see the help for AudioProcessor::getNextAudioBlock()
        //blue();
        channelDataL = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        channelDataR = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);
        
        if(overlay != nullptr){
            if(overlay->numBoards != 0){
                int num = overlay->numBoards;
                //std::cout<<num<<newLine;
                //float amp[num];
                int len[num];
                int ptr[num];
                float * bufferL[num];
                float * bufferR[num];
                
                for(int n = 0; n<num;++n){
                    //amp[n] = overlay->amplitude;
                    len[n] = overlay->boardUI[n]->bufflen;
                    ptr[n] = (overlay->boardUI[n]->buffptr)-(bufferToFill.numSamples);
                    if(ptr[n]<0)
                        ptr[n]+=len[n];
                    //std::cout<<ptr[n]<<newLine; 
                    bufferL[n] = overlay->boardUI[n]->buffL;
                    bufferR[n] = overlay->boardUI[n]->buffR;

                }

                for (int i = 0; i < bufferToFill.numSamples ; ++i)
                {
                    waveformL[waveptr] = 0;
                    waveformR[waveptr] = 0;
                    for(int n = 0; n<num;++n){
                        waveformL[waveptr] += float(bufferL[n][ptr[n]]*overlay->amplitude);
                        waveformR[waveptr] += float(bufferR[n][ptr[n]]*overlay->amplitude);
                        ptr[n]++;
                        if(ptr[n]>=len[n])
                            ptr[n] = 0;
                    }
                    
                    
                    channelDataL[i] = waveformL[waveptr];
                    channelDataR[i] = waveformR[waveptr];
                    
                    waveptr++;
                    if(waveptr>=wavelen)
                        waveptr = 0;
                    
                }
            } else {
                for (int i = 0; i < bufferToFill.numSamples ; ++i){
                    channelDataL[i] = 0;
                    channelDataR[i] = 0;
                }
                for(int x = 0; x<wavelen;++x){
                    waveformL[x] = 0;
                    waveformR[x] = 0;
                }
            }//if(overlay->boardUI !=nullptr)
        } //if(overlay != nullptr)
       
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        
        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);
        
        
        const int centreY = getHeight() / 2;
        
        // draw a representative sinewave
        Path wavePathL;
        Path wavePathR;
        wavePathL.startNewSubPath (0, centreY);
        wavePathR.startNewSubPath (0, centreY);

        float incr = (getWidth()/float(wavelen-1));
        float i = 0.0f;
        int ptr = waveptr;
        for (int x = 0; x < wavelen; x++){
            wavePathL.lineTo (i, centreY + int(waveformL[ptr]*(centreY/2)));
            wavePathR.lineTo (i, centreY + int(waveformR[ptr]*(centreY/2)));
            i += incr;
            ptr++;
            if(ptr>=wavelen){
                ptr=0;
            }

        }
        //std::cout<<getWidth()<<" "<<incr<<" "<<wavelen<<" "<<i<<newLine;

        g.setColour (Colours::red);
        g.setOpacity(0.5f);
        g.strokePath (wavePathL, PathStrokeType (2.0f));
        
        g.setColour (Colours::blue);
        g.setOpacity(0.5f);
        g.strokePath (wavePathR, PathStrokeType (2.0f));

        // You can add your drawing code here!
    }
    
    
    void timerCallback() override
    {
        repaint();
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.);
        //liveAudioDisplayComp.setBounds (0, 0, getWidth(), getHeight());
        if (overlay != nullptr){
            overlay->setBounds(getBounds());
        }
        

    }



private:
    //==============================================================================

    // Your private member variables go here...
    //float buffer[512];
    float * waveformL = nullptr;
    float * waveformR = nullptr;
    int waveptr = 0;
    const int wavelen = 1024;
    
    float * channelDataL = nullptr;
    float * channelDataR = nullptr;
    
    double sampleRate;
    int expectedSamplesPerBlock;
    
    AddBoardButton * overlay = nullptr;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
