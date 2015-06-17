/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Board.cpp"
#include "MainUI.cpp"
#include <chrono>




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
        sampleRate(0.0),
        expectedSamplesPerBlock (0)
    
    {
        setSize (800, 600);
        setOpaque(1);
        

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        //setSampleRate ();
        
        addAndMakeVisible (overlay = new MainUI());
        //startTimerHz (75); // use a timer to keep repainting this component
        startTimerHz (300); // use a timer to keep repainting this component

    }

    ~MainContentComponent()
    {
        
        overlay = nullptr;
        waveformL = nullptr;
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
        //TIMING FUNCTIONS
        //auto time = std::chrono::high_resolution_clock::now();
        
        bufferToFill.clearActiveBufferRegion();
        
        if(waveptr>=wavelen)
            waveptr = 0;
        int start = waveptr;
        
        for(int x = 0; x<bufferToFill.numSamples;x++){
            waveformL[x+start] = 0;
            waveformR[x+start] = 0;
        }
        
        if(overlay != nullptr){
            if(overlay->numBoards != 0){
                int num = overlay->numBoards;
                
                for(int n = 0; n<num;n++){
                    waveptr = start;
                    if(overlay->boardUI[n]->SampleFl){
                        
                        int ptr = overlay->boardUI[n]->sampBuffPtr;
                        int len = overlay->boardUI[n]->samplebuff->getNumSamples();
                        bool end = false;
                        int i = 0;
                        
                        if (overlay->boardUI[n]->playSamp) {
                            overlay->boardUI[n]->sampPlaying = true;
                            ptr = 0;
                        }
                        
                        
                        if(overlay->boardUI[n]->sampPlaying){
                            
                            while (i < bufferToFill.numSamples && !end)
                            {
                                waveformL[waveptr] += overlay->boardUI[n]->samplebuff->getSample(0, ptr)*overlay->boardUI[n]->amplitude;
                                waveformR[waveptr] += overlay->boardUI[n]->samplebuff->getSample(1, ptr)*overlay->boardUI[n]->amplitude;
                            
                                ptr++;
                                 if(ptr>=len){
                                    end = true;
                                    overlay->boardUI[n]->sampPlaying = false;
                                    ptr = 0;
                                }
                            
                                waveptr++;
                                i++;
                            
                            }
                           
                            if(i < bufferToFill.numSamples)
                                waveptr+=bufferToFill.numSamples-i;
                            /*
                            while (i < bufferToFill.numSamples)
                            {
                                //waveformL[waveptr] += 0;
                                //waveformR[waveptr] += 0;
                                    
                                waveptr++;
                                i++;
                            }*/
                            
                            
                            overlay->boardUI[n]->sampBuffPtr = ptr;
                            
                        } else { //if(overlay->boardUI[n]->sampPlaying)
                            
                            waveptr+=bufferToFill.numSamples;
                            /*
                            for(int x = 0; x<bufferToFill.numSamples;x++){
                                waveformL[waveptr] += 0;
                                waveformR[waveptr] += 0;
                                waveptr++;
                            }*/
                        }
                    
                    } else { //if(overlay->boardUI[n]->SampleFl)
                    
                        int len = overlay->boardUI[n]->bufflen;
                        int ptr = (overlay->boardUI[n]->buffptr)-(bufferToFill.numSamples);
                        if(ptr<0)
                            ptr+=len;

                        float *bufferL = overlay->boardUI[n]->buffL;
                        float *bufferR = overlay->boardUI[n]->buffR;
                    
                        for (int i = 0; i < bufferToFill.numSamples ; i++)
                        {
                        
                            waveformL[waveptr] += float(bufferL[ptr]);
                            waveformR[waveptr] += float(bufferR[ptr]);
                        
                            ptr++;
                            if(ptr>=len)
                                ptr = 0;
                            waveptr++;

                        }//for (int i = 0; i < bufferToFill.numSamples ; ++i)
                        
                    } //if(overlay->boardUI[n]->SampleFl)
                }//for(int n = 0; n<num;++n)
                
                
            }//if(overlay->boardUI !=nullptr)

            bufferToFill.buffer->copyFrom(0, bufferToFill.startSample, &waveformL[start], bufferToFill.numSamples,overlay->amplitude);
            bufferToFill.buffer->copyFrom(1, bufferToFill.startSample, &waveformR[start], bufferToFill.numSamples,overlay->amplitude);
            
        } //if(overlay != nullptr)
        // TIMING FUNCTIONS
        //auto time2 = std::chrono::high_resolution_clock::now();
        //std::cout<<std::chrono::duration_cast<std::chrono::microseconds>(time2-time).count()<<newLine;
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
        
        Path wavePathL;
        Path wavePathR;
        wavePathL.startNewSubPath (0, centreY);
        wavePathR.startNewSubPath (0, centreY);

        float incr = (getWidth()/float(wavelen-1));
        float i = 0.0f;
        for (int x = 0; x < wavelen; x++){
            wavePathL.lineTo (i, centreY + int(waveformL[x]*(centreY/2)));
            wavePathR.lineTo (i, centreY + int(waveformR[x]*(centreY/2)));
            i += incr;
        }

        g.setColour (Colours::steelblue);
        g.setOpacity(0.5f);
        g.strokePath (wavePathR, PathStrokeType (3.0f));
        
        g.setColour (Colours::red);
        g.setOpacity(0.5f);
        g.strokePath (wavePathL, PathStrokeType (3.0f));
        
        

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
        if (overlay != nullptr){
            overlay->setBounds(getBounds());
        }
        

    }



private:
    //==============================================================================

    // Your private member variables go here...
    ScopedPointer<float> waveformL = nullptr;
    ScopedPointer<float> waveformR = nullptr;
    int waveptr = 0;
    const int wavelen = 1536;
    
    double sampleRate;
    int expectedSamplesPerBlock;
    
    ScopedPointer<MainUI> overlay = nullptr;

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
