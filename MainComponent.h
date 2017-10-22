/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "GRT/GRT.h"
#include "../../Libs/stuff_From_Dom/CsvParser.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent : public AudioAppComponent,
                             public HighResolutionTimer,
                             public Button::Listener,
                             public Slider::Listener,
                             public ChangeListener

{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;
    void hiResTimerCallback() override;
    void buttonClicked (Button* button) override;
    void buttonStateChanged (Button* button)  override;
    
    void sliderValueChanged (Slider* slider) override;
    void configureSensorChannels();
    
    // AUDIO METHODS
    
  void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /** Allows the source to release anything it no longer needs after playback has stopped.
     
     This will be called when the source is no longer going to have its getNextAudioBlock()
     method called, so it should release any spare memory, etc. that it might have
     allocated during the prepareToPlay() call.
     
     Note that there's no guarantee that prepareToPlay() will actually have been called before
     releaseResources(), and it may be called more than once in succession, so make sure your
     code is robust and doesn't make any assumptions about when it will be called.
     
     @see prepareToPlay, getNextAudioBlock
     */
    void releaseResources() override;
    
    /** Called repeatedly to fetch subsequent blocks of audio data.
     
     After calling the prepareToPlay() method, this callback will be made each
     time the audio playback hardware (or whatever other destination the audio
     data is going to) needs another block of data.
     
     It will generally be called on a high-priority system thread, or possibly even
     an interrupt, so be careful not to do too much work here, as that will cause
     audio glitches!
     
     @see AudioSourceChannelInfo, prepareToPlay, releaseResources
     */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    // Change Listener callback
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    
    
    
    
    
private:
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };
    
    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;
            
            switch (state)
            {
                case Stopped:                           // [3]
                    transportSource.setPosition (0.0);
                    break;
                    
                case Starting:                          // [4]
                    transportSource.start();
                    break;
                    
                case Playing:                           // [5]
                    break;
                    
                case Stopping:                          // [6]
                    transportSource.stop();
                    break;
            }
        }
    }
    
    GRT::GestureRecognitionPipeline pipe;
    GRT::TimeSeriesClassificationData trainingData;
    GRT::DTW dtwObject;
    GRT::HMM hmmObject;
    OSCSender sender;
    OwnedArray<TextButton> windowButtons;
    OwnedArray<TextButton> channelButtons;
    OwnedArray<Label> windowLabels;
    Slider posSlider;
    Slider classLabelSlider;
    Slider WindowSizeSelector;
    Slider sigmaSlider;
    Slider downSampleSlider;
    TextEditor fileFeedback;
    TextEditor audioFileFeedback;
    TextEditor grtFeedback;
    CsvParser parser;
    OwnedArray<Array<float>> sensorChannels;
    OwnedArray<Array<float>> sampleSet;
    OwnedArray<GRT::MatrixFloat> matrixFloatsArray;
    int rowCount = 0;
    int numFiles = 0;
    int columnToLoadInto;
    int columnToLoadFrom;
    int numChannels = 2;
    double playbackFrac;
    double audioFileLength;
    int dataLengthInSamps = 0;
    StringArray labelStrings;
    StringArray filenames;
    String currentFile;
    Array<bool> columnChecks;
    bool takeSample = false;
    bool dtwRunning = false;
    TransportState state;
    
    GRT::Vector<GRT::Float> tempVector;
    GRT::VectorFloat inputVector;
    GRT::Float* pVecData;
    GRT::CircularBuffer<GRT::VectorFloat> cBuff;
    GRT::MatrixFloat currentMatrix;

    
    
    // Audio Objects
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    
  
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
