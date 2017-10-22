/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include <chrono>
#include <thread>


//==============================================================================
MainContentComponent::MainContentComponent()
{
//    pipe << GRT::DTW(); /* The Pipe object uses the "<<" operator to load 'modules into it'. This DTW object
//                            Is the type of classifier that the pipe will use    */
//    
//    
//
//    pipe << GRT::LowPassFilter(); // This is a pre-processing module that the pipe will use.
//    GRT::ClassificationData myFile; // The Pipe will be trained using the ClassificationData object.
//    pipe.train(myFile); // I can train it upon loading the app.
//
    
    tempVector.resize(2);
    inputVector.resize(2);
    inputVector.clear();
    GRT::VectorFloat dummyVec;
    dummyVec.push_back(0.0);
    dummyVec.push_back(0.0);
    cBuff.resize(200, dummyVec);
    
    hmmObject.setHMMType(HMM_CONTINUOUS);
    hmmObject.setAutoEstimateSigma(true);
    pipe.setClassifier(hmmObject);
    
    
//    currentMatrix;
    
//    pipe << GRT::ClassLabelFilter(5, 10);
    pipe << GRT::ClassLabelTimeoutFilter(200, GRT::ClassLabelTimeoutFilter::ALL_CLASS_LABELS);
    static const char* buttonNames[] =
    {
        "Load File",
        "Simulate Playback",
        "Stop Playback",
        "Load Selected To GRT",
        "Config Channels",
        "Add Audio File",
        "Take Sample",
        "Save Sample",
        "Clear Sample",
        "Train Model",
        "Run Model",
        "Add Training Sample",
        "Load Model",
        "Clear set"
    };
    
    static const char* labelNames[] =
    {
        "TimeStamp",
        "Sensors ",
        "Playback State",
        "Class Label"
    };
    
    for (int i = 0; i < numElementsInArray(buttonNames); i ++)
    {
        TextButton* newButton = new TextButton(buttonNames[i]);
        newButton->addListener(this);
        newButton->setButtonText(buttonNames[i]);
        addAndMakeVisible(newButton);
        windowButtons.add(newButton);
    }

    
    for (int i = 0; i < numElementsInArray(labelNames); i ++)
    {
        labelStrings.add(labelNames[i]);
        Label* newLabel = new Label;
        newLabel->setColour(Label::ColourIds::textColourId, Colours::papayawhip);
        newLabel->setText(labelNames[i], NotificationType::dontSendNotification);
        addAndMakeVisible(newLabel);
        windowLabels.add(newLabel);
    }
    
    windowLabels[1]->setVisible(false);
    
    windowButtons[1]->setColour(TextButton::ColourIds::buttonColourId, Colours::forestgreen);
    windowButtons[1]->setColour(TextButton::ColourIds::buttonOnColourId, Colours::limegreen);
    windowButtons[1]->setEnabled(false);
    
    windowButtons[2]->setColour(TextButton::ColourIds::buttonColourId, Colours::firebrick);
    windowButtons[2]->setColour(TextButton::ColourIds::buttonOnColourId, Colours::indianred);
    windowButtons[2]->setEnabled(false);
    
    windowButtons[4]->setColour(TextButton::buttonColourId, Colours::gainsboro);
    windowButtons[4]->setColour(TextButton::ColourIds::textColourOffId, Colours::black);
    windowButtons[4]->setEnabled(false);
    
    windowButtons[6]->setColour(TextButton::buttonColourId, Colours::lightskyblue);
    windowButtons[6]->setEnabled(false);
    
    windowButtons[7]->setColour(TextButton::buttonColourId, Colours::lightskyblue);
    windowButtons[7]->setEnabled(false);
    
    windowButtons[8]->setColour(TextButton::buttonColourId, Colours::indianred);
    windowButtons[8]->setEnabled(false);
    
    windowButtons[9]->setColour(TextButton::buttonColourId, Colours::lightskyblue);
    windowButtons[9]->setEnabled(false);
    
    windowButtons[10]->setColour(TextButton::buttonColourId, Colours::lightskyblue);
    windowButtons[10]->setEnabled(false);
    
    windowButtons[11]->setColour(TextButton::buttonColourId, Colours::lightskyblue);
    windowButtons[11]->setEnabled(false);
    
    windowButtons[12]->setColour(TextButton::buttonColourId, Colours::lightskyblue);
    windowButtons[12]->setEnabled(false);
    
    
    addAndMakeVisible(&fileFeedback);
    fileFeedback.setReadOnly(true);
    fileFeedback.setColour(TextEditor::ColourIds::backgroundColourId, Colours::darkblue);
    fileFeedback.setTextToShowWhenEmpty("No data file selected", Colours::firebrick);
    fileFeedback.setMultiLine(true);
    
    addAndMakeVisible(&audioFileFeedback);
    audioFileFeedback.setReadOnly(true);
    audioFileFeedback.setColour(TextEditor::ColourIds::backgroundColourId, Colours::forestgreen);
    audioFileFeedback.setColour(TextEditor::ColourIds::textColourId, Colours::lightgoldenrodyellow);
    audioFileFeedback.setTextToShowWhenEmpty("No audio file selected", Colours::firebrick);
    audioFileFeedback.setMultiLine(true);
    
    addAndMakeVisible(&grtFeedback);
    grtFeedback.setReadOnly(true);
    grtFeedback.setColour(TextEditor::ColourIds::backgroundColourId, Colours::palevioletred);
    grtFeedback.setColour(TextEditor::ColourIds::textColourId, Colours::lightgoldenrodyellow);
    grtFeedback.setTextToShowWhenEmpty("No Sample data", Colours::black);
    grtFeedback.setMultiLine(true);
    
    
    
    formatManager.registerBasicFormats();       // [1]
    transportSource.addChangeListener (this);   // [2]
    
    setAudioChannels (0, 1); // Mono
    
    
//    if(!(sender.connect("127.0.0.1", 9001))) // For the Processing APP
//    {
//        DBG("ERROR connecting OSC");
//    }
    
    if(!(sender.connect("192.168.7.2", 7562))) // For the Bela.
    {
        DBG("ERROR connecting OSC");
    }
    
    
    posSlider.addListener(this);
    posSlider.setSliderStyle(Slider::LinearHorizontal);
    posSlider.setRange(0.0, 1.0);
    addAndMakeVisible(&posSlider);
    
    classLabelSlider.addListener(this);
    classLabelSlider.setSliderStyle(Slider::IncDecButtons);
    classLabelSlider.setRange(0.0, 5.0, 1.0);
    addAndMakeVisible(&classLabelSlider);
    classLabelSlider.setEnabled(false);
    classLabelSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::lightskyblue);
    
    WindowSizeSelector.addListener(this);
    WindowSizeSelector.setSliderStyle(Slider::IncDecButtons);
    WindowSizeSelector.setRange(10.0, 1000.0, 1.0);
    addAndMakeVisible(&WindowSizeSelector);
    WindowSizeSelector.setEnabled(true);
    WindowSizeSelector.setColour(Slider::ColourIds::backgroundColourId, Colours::lightskyblue);
    
    sigmaSlider.addListener(this);
    sigmaSlider.setSliderStyle(Slider::IncDecButtons);
    sigmaSlider.setRange(1.0, 50.0, 1.0);
    addAndMakeVisible(&sigmaSlider);
    sigmaSlider.setEnabled(true);
    sigmaSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::lightskyblue);
   
    downSampleSlider.addListener(this);
    downSampleSlider.setSliderStyle(Slider::IncDecButtons);
    downSampleSlider.setRange(1.0, 20.0, 1.0);
    addAndMakeVisible(&downSampleSlider);
    downSampleSlider.setEnabled(true);
    downSampleSlider.setColour(Slider::ColourIds::backgroundColourId, Colours::lightskyblue);
    
    windowLabels[3]->setJustificationType(Justification::centred);
    
    setSize (800, 600);
    
}

MainContentComponent::~MainContentComponent()
{
    
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));
}

void MainContentComponent::resized()
{
    Rectangle<int> button = getLocalBounds().removeFromTop(getHeight() / 4).removeFromLeft(getWidth() / 3).reduced(20, 20);
    
    windowButtons[0]->setBounds(button.removeFromLeft(button.getWidth() / 2).reduced(5, 5));
    windowButtons[13]->setBounds(button.reduced(5, 5));
    
    
    windowLabels[1]-> setBounds(20, 130, 200, 50);

    button = getLocalBounds().removeFromTop(getHeight() / 5).removeFromRight(getWidth() / 4).reduced(20, 20);
    
    windowButtons[1]->setBounds(button);
    
    button = getLocalBounds().removeFromRight(getWidth() / 4);
    
    button.removeFromTop(windowButtons[1]->getHeight() * 1.5);
    
    windowButtons[2]->setBounds(button.removeFromTop(windowButtons[1]->getHeight()).reduced(20, 0));
    
    button = getLocalBounds().removeFromRight(getWidth() / 4);
    
    button.removeFromTop(windowButtons[1]->getHeight() * 1.5);
    
    button.removeFromTop(windowButtons[1]->getHeight() * 1.5);
    
    windowButtons[5]->setBounds(button.removeFromTop(windowButtons[1]->getHeight()).reduced(20, 0));

    button = getLocalBounds(). removeFromLeft(windowButtons[0]->getWidth() * 1.8);
    
    button.removeFromBottom(20);
    
    windowButtons[3]->setBounds(button.removeFromBottom(windowButtons[0]->getHeight() - 30).reduced(20, 10));
    
    windowButtons[4]->setBounds(button.removeFromBottom(windowButtons[0]->getHeight() - 30).reduced(20, 10));
    
    downSampleSlider.setBounds(button.removeFromBottom(windowButtons[0]->getHeight() - 30).reduced(20, 10));
    
    sigmaSlider.setBounds(button.removeFromBottom(windowButtons[0]->getHeight() - 30).reduced(20, 10));

    fileFeedback.setBounds((getWidth() / 2) - ((getWidth() / 3.5) / 2), 20, getWidth() / 3.5, windowButtons[0]->getHeight());
    
    audioFileFeedback.setBounds((getWidth() / 2) - ((getWidth() / 3.5) / 2), fileFeedback.getHeight() * 1.2, fileFeedback.getWidth(), fileFeedback.getHeight());
    
    posSlider.setBounds(20, getHeight() - 40, getWidth() - 40 , 40);
    
    for (int i = 0; i < channelButtons.size(); i ++)
    {
        channelButtons[i]->setBounds(20, (60 * i) + 170, 50, 50);
    }
    
    button = getLocalBounds().removeFromRight(getWidth() / 4);
    
    button.removeFromBottom(posSlider.getHeight());
    
    windowButtons[8]->setBounds(button.removeFromBottom(windowButtons[5]->getHeight() / 2).reduced(20, 0));
    
    button.removeFromBottom(posSlider.getHeight() / 1.5);
    
    windowButtons[7]->setBounds(button.removeFromBottom(windowButtons[5]->getHeight() / 2).reduced(20, 0));
    
    button.removeFromBottom(posSlider.getHeight() / 1.5);
    
    windowButtons[6]->setBounds(button.removeFromBottom(windowButtons[5]->getHeight() / 2).reduced(20, 0));
    
    Rectangle<int> grtSection = getLocalBounds().removeFromBottom(getHeight() / 1.6);
    
    grtSection.removeFromLeft(getWidth() / 4);
    grtSection.removeFromRight(getWidth() / 4);
    grtSection.removeFromBottom(posSlider.getHeight() / 2);
    grtSection.reduce(30,10);
    
    grtFeedback.setBounds(grtSection.removeFromBottom(posSlider.getHeight() * 1.5).reduced(10, 10));
    
    WindowSizeSelector.setBounds(grtSection.removeFromBottom(grtSection.getHeight() / 8));
    classLabelSlider.setBounds(grtSection.removeFromBottom(grtSection.getHeight() / 8));
    windowLabels[3]->setBounds(grtSection.removeFromBottom(grtSection.getHeight() / 8));
    Rectangle<int> grtSubSection = grtSection.removeFromBottom(grtSection.getHeight() / 2);
    
    windowButtons[9]->setBounds(grtSubSection.removeFromLeft(grtSubSection.getWidth() / 2).reduced(20 , 20));
    windowButtons[10]->setBounds(grtSubSection.removeFromRight(windowButtons[9]->getWidth()).reduced(0, 20));
    
    windowButtons[11]->setBounds(grtSection.removeFromLeft(grtSection.getWidth() / 2).reduced(20 , 20));
    windowButtons[12]->setBounds(grtSection.removeFromRight(windowButtons[9]->getWidth()).reduced(0, 20));

}
void MainContentComponent::hiResTimerCallback() // Send data out as OSC message.
{
    if (!(sensorChannels.isEmpty()))
    {
//        if(! sender.send("/juce/bodydata", (float)sensorChannels[0]->getUnchecked(rowCount), (float)sensorChannels[1]->getUnchecked(rowCount)))
//           {
//               AlertWindow::showMessageBoxAsync (
//                                                 AlertWindow::WarningIcon,
//                                                 "Connection error",
//                                                 "Error: sending OSC message.",
//                                                 "OK"); // For the Processing App %%%%%%%%%
//           }
        
        if(! sender.send("/osc-test", (float)sensorChannels[0]->getUnchecked(rowCount), (float)sensorChannels[1]->getUnchecked(rowCount)))
        {
            AlertWindow::showMessageBoxAsync (
                                              AlertWindow::WarningIcon,
                                              "Connection error",
                                              "Error: sending OSC message.",
                                              "OK");
        }
        
        if (takeSample) // Taking the sample if the button is pressed.
        {
            sampleSet[0]->add(sensorChannels[0]->getUnchecked(rowCount));
            sampleSet[1]->add(sensorChannels[1]->getUnchecked(rowCount));
            
        }
        
        if (dtwRunning)
        {

            // Add one sample from each sensor to the VectorFloat
            inputVector.push_back(sensorChannels[0]->getUnchecked(rowCount));
            inputVector.push_back(sensorChannels[1]->getUnchecked(rowCount));
            
            // Add the current samples from the sensors to the 200 sample length Circular Buffer.
            cBuff.push_back(inputVector);
            
//            DBG("[DBG] RING BUFFER SIZE = " + String(cBuff.getSize()));

            

            if (!(rowCount % (int)WindowSizeSelector.getValue()))
            
            {
//                GRT::MatrixFloat currentMatrix (cBuff.getData());
                currentMatrix = cBuff.getData();
//                DBG("[DBG] Matrix float Rows : " + String(currentMatrix.getNumRows()) + "  Cols : " + String(currentMatrix.getNumCols()));
                
                if(pipe.predict_(currentMatrix))
                {
//                    std::cout << rowCount;
                }
                
                else
                {
//                    std::cout << rowCount << "  FAILURE ";
                }
                
                int classLabel = pipe.getPredictedClassLabel();
                
//                if (classLabel == 1)
//                {
//                    std::cout << "  STILL   ";
//                }
//                
//                else if (classLabel == 2)
//                {
//                    std::cout << "  ONSET  ";
//                }
                
                if (classLabel == 2)
                {
                    std::cout << rowCount << " ONSET " << std::endl;
                }
                
//                std::cout << std::endl;
            }
            
            inputVector.clear();
            currentMatrix.clear();
        }
        
        if ((rowCount + 1) < dataLengthInSamps)
        {
            rowCount++;
        }
        else
        {
            rowCount = 0;
            stopTimer();
            
            AlertWindow::showMessageBoxAsync (
                                              AlertWindow::WarningIcon,
                                              "DataFile Finished",
                                              "Simulation Complete",
                                              "OK");
        }
        
        if (!(rowCount % 40)) // updating every 40 ms (25 fps)
        {
            const MessageManagerLock mmlock;
            posSlider.setValue(playbackFrac * rowCount);
        }
    }
}

void MainContentComponent::buttonClicked (Button* button)
{
    if (button == windowButtons[0]) // File Loading
    {
        ImagePreviewComponent imagePreview;
        imagePreview.setSize (200, 200);
        
        FileChooser fc ("Choose a Txt to open...",
                        File::getSpecialLocation (File::userDesktopDirectory).getChildFile("Internship/"),
                        "*.txt");
        
        if (fc.browseForFileToOpen(&imagePreview))
        {
            String chosen;
            for (int i = 0; i < fc.getResults().size(); ++i)
                chosen << fc.getResults().getReference (i).getFullPathName() << "\n";
            
            AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                              "File Chooser...",
                                              "You picked: " + chosen);
            
            currentFile = fc.getResult().getFileName();
            
            fileFeedback.setText("File currently open is: " + currentFile);
            
            GRT::MatrixFloat* newMatrixFloat = new GRT::MatrixFloat;
            jassert(newMatrixFloat->load(fc.getResult().getFullPathName().toStdString(), '\''));
            matrixFloatsArray.add(newMatrixFloat);
            numFiles ++;
            filenames.add(String(fc.getResult().getFileName()));
            bool newBool = true;
            columnChecks.add(newBool);
            
            TextButton* newButton = new TextButton(String(numFiles));
            newButton->addListener(this);
            newButton->setToggleState(false,NotificationType::dontSendNotification);
            newButton->setButtonText(String(numFiles));
            addAndMakeVisible(newButton);
            channelButtons.add(newButton);
            resized();
            
            if (numFiles > 0)
            {
                windowLabels[1]->setVisible(true);
                windowButtons[4]->setEnabled(true);
            }
        }
    }
    
        else if (button == windowButtons[1]) // Simulate playback.
    {
        startTimer(1); // Play a sample every millisecond
        changeState(Starting);
        
        DBG("BAM!");
    }
    
    else if (button == windowButtons[2]) // Stop Playback.
    {
        stopTimer();
        rowCount = 0; // reset the playback position of file.
        posSlider.setValue(0);
        changeState(Stopping);
        DBG("Stopped!");
        
        if (sampleSet[0]->size() > 30)
        {
            windowButtons[7]->setEnabled(true);
            windowButtons[8]->setEnabled(true);
            windowButtons[9]->setEnabled(true);
            windowButtons[11]->setEnabled(true);
            windowButtons[12]->setEnabled(true);
            classLabelSlider.setEnabled(true);
            grtFeedback.setText("New Sample Length : " + String(sampleSet[0]->size()));
        }
    }
    
    else if (button == windowButtons[3])
    {
        int rowNum = 0;
        int colNum = 0;
        int totalCols = 0;
        
        if (! matrixFloatsArray.isEmpty())
        {
            rowNum = matrixFloatsArray[0]->getCol(1).getSize();
            DBG("ROWS = : " + String(rowNum));
            
            for (int i = 0; i < matrixFloatsArray.size(); i++)
            {
                colNum = (matrixFloatsArray[i]->getSize() / rowNum);
                totalCols += colNum;
            }
            
            DBG("TOTAL = : " + String(totalCols));
        }
        
        GRT::Matrix<GRT::Float> compMatrix(rowNum, matrixFloatsArray.size());
        
        for (int a = 0; a < matrixFloatsArray.size(); a ++)
        {
            if (columnChecks.getUnchecked(a) == true)
            {
                GRT::Vector<GRT::Float> newCol(rowNum);
                GRT::Float* pdata = newCol.getData();
                GRT::Float* getData = matrixFloatsArray[a]->getCol(1).getData();
                
                for (int i = 0; i < rowNum; i ++)
                {
                    *pdata = *getData;
                    pdata++;
                    getData++;
                }
                
                compMatrix.setColVector(newCol, a);
            }
        }
        
        GRT::MatrixFloat tempMatrixFloat (compMatrix);
        
        tempMatrixFloat.print();
       
        
        ImagePreviewComponent savePreview;
        savePreview.setSize (200, 200);
        
        FileChooser fc ("Choose a Location to save to...",
                        File::getSpecialLocation (File::userHomeDirectory).getChildFile("Internship/"));
        
        if (fc.browseForFileToSave(true))
        {
            String chosen;
    
            jassert(tempMatrixFloat.save(fc.getResult().getFullPathName().toStdString()));
        }
    }
    
    else if (button == windowButtons[4])
    {
        configureSensorChannels();
    }
    
    else if (button == windowButtons[5])
    {
        FileChooser chooser ("Select a Wave file to play...",
                              File::getSpecialLocation (File::userDesktopDirectory).getChildFile("Internship/"),
                             "*.wav");
        
        if (chooser.browseForFileToOpen())
        {
            File file (chooser.getResult());
            AudioFormatReader* reader = formatManager.createReaderFor (file);
            
            if (reader != nullptr)
            {
                ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
                transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
                
                readerSource = newSource.release();
            }
            audioFileFeedback.setText("Attatched Audio File is : " + chooser.getResult().getFileName());
            audioFileLength = transportSource.getLengthInSeconds();
            
            DBG("Length in Seconds is : " + String(audioFileLength));
        }
    }
    
    else if (button == windowButtons[7]) // Saving Sample set
    {
        
        if (!sampleSet.isEmpty())
        {
            ImagePreviewComponent savePreview;
            savePreview.setSize (200, 200);
            
            FileChooser fc ("Choose a Location to save Sample to...",
                            File::getSpecialLocation (File::userHomeDirectory).getChildFile("Internship/"));
            
            if (fc.browseForFileToSave(true))
            {
                int rowNum = sampleSet[0]->size();
                
                GRT::Matrix<GRT::Float> sampMatrix(rowNum, sampleSet.size());

                for (int a = 0; a < sampleSet.size(); a ++)
                {
                    GRT::Vector<GRT::Float> newCol(rowNum);
                    GRT::Float* pdata = newCol.getData();
                    
                    for (int i = 0; i < rowNum; i ++) // traverse the sample columns, storing data using the pointer to the newCol Vector.
                    {
                        *pdata = sampleSet[a]->getUnchecked(i);
                        pdata ++;
                    }
                    
                    sampMatrix.setColVector(newCol, a);
                } // end nested for loop/
                
                GRT::MatrixFloat sampMatrixFloat(sampMatrix);
                
//                jassert(sampMatrixFloat.save(fc.getResult().getFullPathName().toStdString()));
                trainingData.save(fc.getResult().getFullPathName().toStdString());
            }
            
        }
    }
    
    else if (button == windowButtons[8]) // clearing sample set.
    {
        if (!sampleSet.isEmpty())
        {
            for (int i = 0; i < sampleSet.size(); i ++)
            {
                sampleSet[i]->clear();
            }
            grtFeedback.setText("Sample set deleted");
        }
        else
        {
            grtFeedback.setText("No sample data to clear!");
        }
    }
    
    else if (button == windowButtons[9]) // TRAINING the pipe with the sample data and saving it.
    {
        int numTrainingSamps = trainingData.getNumSamples();
        
        if (numTrainingSamps != 0) // make sure the training data has some data.
        {
            if (hmmObject.train_(trainingData))
            {
                grtFeedback.setText("Pipeline sucessfully trained with " + String(numTrainingSamps) + "samples");
                windowButtons[10]->setEnabled(true);
                
            }
            // Then save the trained pipeline to a file.
            ImagePreviewComponent savePreview;
            savePreview.setSize (200, 200);
            
            FileChooser fc ("Choose a Location to save trained DTW object to...",
                            File::getSpecialLocation (File::userHomeDirectory).getChildFile("Internship/"));
            
            if (fc.browseForFileToSave(true))
            {
                hmmObject.save(fc.getResult().getFullPathName().toStdString());
            }

        }
    }
    
    else if (button == windowButtons[10]) // Running the DTW object with new samples
    {
//         RUNNNING REALTIME PREDICITION ##########################################
        windowButtons[10]->setToggleState(!windowButtons[10]->getToggleState(), NotificationType::dontSendNotification);
        
//        if (windowButtons[10]->getToggleState())
//        {
//            dtwRunning = true;
//            DBG("[DTW RUNNING]");
//        }
//        
//        else
//        {
//            dtwRunning = false;
//            DBG("[DTW OFF]~~~~");
//
//        }
        
//         RUNNING STATIC PREDICTION ON GRABBED SAMPLES ##############################
        if (! sampleSet.isEmpty())
        {
            int rowNum = sampleSet[0]->size();
            
            GRT::VectorFloat inVec;
            
            inVec.push_back(0.0);
            inVec.push_back(0.0);
            
            cBuff.resize(300, inVec);
            
            inVec.clear();
            
            for (int a = 0; a < rowNum; a ++) //
            {
                for (int i = 0; i < sampleSet.size(); i ++) // counting across first...
                {
                    inVec.push_back(sampleSet[i]->getUnchecked(a)); // So each Vector float will be a snapshot in time (one sample from each column/sensor)
                }
                cBuff.push_back(inVec);
                inVec.clear();
                
            } // end nested for loop/
            
//            GRT::MatrixFloat newTimeseries(cBuff.getData()); // Using overloaded constructor to convert the circular buffer into a MatrixFloat object to go to the pipeline.
            
            GRT::MatrixFloat newTimeseries;
            newTimeseries = cBuff.getData();
            
            DBG("[DBG] Ring buffer size : " + String(cBuff.getSize()));
            DBG("[DBG] Matrix Columns : " + String(newTimeseries.getNumCols()) + "  Rows : " + String(newTimeseries.getNumRows()));
            
            hmmObject.predict(newTimeseries);
            
            if (! hmmObject.predict_(newTimeseries))
            {
                DBG("Failed to make a prediction");
            }
            
            else
            {
                grtFeedback.setText("Output class is : " + String(hmmObject.getPredictedClassLabel()) + " Likelihood = " + String(hmmObject.getMaximumLikelihood()));
//                dtwObject.getMaximumLikelihood();
            }
            
        }
//
        
//        ImagePreviewComponent savePreview;
//        savePreview.setSize (200, 200);
//        
//        FileChooser fc ("Choose a Location to save trained DTW object to...",
//                        File::getSpecialLocation (File::userHomeDirectory).getChildFile("Internship/"));
//        
//        if (fc.browseForFileToSave(true))
//        {
//            FileOutputStream fos(fc.getResult());
//            
//            if (! sensorChannels.isEmpty())
//            {
//                GRT::VectorFloat inputVec(2);
//                inputVector.clear();
//                
//                for (int i = 0; i < sensorChannels[0]->size(); i ++)
//                {
//                    inputVector.push_back(sensorChannels[0]->getUnchecked(i));
//                    inputVector.push_back(sensorChannels[1]->getUnchecked(i));
//                    
//                    fos << i << "   ";
//                    
//                    if(pipe.predict_(inputVector))
//                    {
//                        int predictedClass = pipe.getPredictedClassLabel();
//                        
//                        fos << "Prediction:     ";
//                        
//                        if (predictedClass == 1)
//                        {
//                            fos << "[STILL] ";
//                        }
//                        
//                        else if (predictedClass == 2)
//                        {
//                            fos << "[ONSET] ";
//                        }
//                        
//                        else
//                        {
//                            fos << "NIL";
//                        }
//                        
//                        int unProcessedClass = pipe.getUnProcessedPredictedClassLabel();
//                        
//                        if (unProcessedClass == 1)
//                        {
//                            fos << "    (U)[STILL] ";
//                        }
//                        
//                        else if (unProcessedClass == 2)
//                        {
//                            fos << "    (U)[ONSET] ";
//                        }
//                    }
//                    
//                    else
//                    {
//                        fos << "No Prediction Made";
//                    }
//                    
//                    inputVector.clear();
//                    
//                    fos << newLine;
//                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
//                }
//                
//                fos << "End Of Set";
//            }
//            
//        }
    }
    
    else if (button == windowButtons[11]) // Adding sample to the Time Series classifiction data.
    {
        if (! sampleSet.isEmpty())
        {
            int rowNum = sampleSet[0]->size();
            
            GRT::Matrix<GRT::Float> sampMatrix(rowNum, sampleSet.size());
            
            for (int a = 0; a < sampleSet.size(); a ++)
            {
                GRT::Vector<GRT::Float> newCol(rowNum);
                GRT::Float* pdata = newCol.getData();
                
                for (int i = 0; i < rowNum; i ++) // traverse the sample columns, storing data using the pointer to the newCol Vector.
                {
                    *pdata = sampleSet[a]->getUnchecked(i);
                    pdata ++;
                }
                
                sampMatrix.setColVector(newCol, a);
            } // end nested for loop/
            
            GRT::MatrixFloat sampMatrixFloat(sampMatrix);
            
            if(trainingData.addSample((int)classLabelSlider.getValue(), sampMatrixFloat))
            {
                grtFeedback.setText("Training data now has " + String(trainingData.getNumSamples()) + "Samples");
            }
        }
    }
        
    else if (button == windowButtons[12]) // load HMM Model from file
    {
        ImagePreviewComponent imagePreview;
        imagePreview.setSize (200, 200);
        
        FileChooser fc ("Choose a HMM object to open...",
                        File::getSpecialLocation (File::userDesktopDirectory).getChildFile("Internship/"));
        
        if (fc.browseForFileToOpen(&imagePreview))
        {
            String chosen;
            for (int i = 0; i < fc.getResults().size(); ++i)
                chosen << fc.getResults().getReference (i).getFullPathName() << "\n";
            
            AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                              "File Chooser...",
                                              "You picked: " + chosen);
            
//            GRT::TimeSeriesClassificationData newData;
            
            if(trainingData.load(fc.getResult().getFullPathName().toStdString()))
            {
                grtFeedback.setText("Success loading T data " + String(trainingData.getNumSamples()));
            }
            
//            if(hmmObject.train(newData))
//            {
//                
////                if (pipe.setClassifier(dtwObject))
////                {
////                    grtFeedback.setText("Successfully loaded a DTW object with " + String(dtwObject.getNumTemplates()) + "Templates inside");
////                    windowButtons[10]->setEnabled(true);
////                    DBG("Is Pipe Trained? = " + String(pipe.getTrained()) + "Input Dimension size is - " + String(pipe.getInputVectorDimensionsSize()));
//////                    int avgTemp = dtwObject.averageTemplateLength;
//           // }
////
//                grtFeedback.setText("Successfully loaded a HMM object with " + String(hmmObject.getContinuousModels().getSize()) + "Models (Templates) inside");
//                                    windowButtons[10]->setEnabled(true);
//            }
            
        }
    }
    
    else if (button == windowButtons[13]) // clear The data files to make way for a new set.
    {
        matrixFloatsArray.clear();
        channelButtons.clear();
        
        numFiles = 0;
        filenames.clear();
        columnChecks.clear();
        resized();
        
        fileFeedback.setText("Data files set cleared.");
        
    }
    
    for (int i = 0; i < channelButtons.size(); i ++)
    {
        if (button == channelButtons[i])
        {
            channelButtons[i]->setToggleState(!channelButtons[i]->getToggleState(), NotificationType::dontSendNotification);
            fileFeedback.setText(filenames[i]);
            columnChecks.set(i, channelButtons[i]->getToggleState());
            
            DBG("File " + String(i) + " is " + String(columnChecks.getUnchecked(i)));
        }
    }

}

void MainContentComponent::buttonStateChanged (Button* button)
{
    if (button == windowButtons[6])
    {
        if (windowButtons[6]->isDown())
        {
            takeSample = true;
            DBG("DOWN");
        }
        else if (!windowButtons[6]->isDown())
        {
            takeSample = false;
            DBG("UP");

        }
    }
}
void MainContentComponent::sliderValueChanged (Slider* slider)
{
    if(slider == & posSlider)
    {
        rowCount = (posSlider.getValue() * dataLengthInSamps);
    }
    
    else if (slider == &sigmaSlider)
    {
        hmmObject.setSigma(sigmaSlider.getValue());
    }
    
    else if (slider == &downSampleSlider)
    {
        hmmObject.setDownsampleFactor(downSampleSlider.getValue());
    }
}

void MainContentComponent::configureSensorChannels()
{
    sampleSet.clear();
    
    for (int a = 0; a < matrixFloatsArray.size(); a ++)
    {
        if (columnChecks.getUnchecked(a) == true)
        {
            Array<float>* newChannel = new Array<float>;
            int rowNum = matrixFloatsArray[0]->getCol(1).getSize();
            GRT::Float* getData = matrixFloatsArray[a]->getCol(1).getData();
            
            
            for (int i = 0; i < rowNum; i ++)
            {
                newChannel->add(*getData);
                getData++;
            }
            
            sensorChannels.add(newChannel);
        }
    }
    
    windowButtons[6]->setEnabled(true);
    
    for (int i = 0; i < sensorChannels.size(); i ++)
    {
        Array<float>* newChannel = new Array<float>;
        sampleSet.add(newChannel);
    }
    
    dataLengthInSamps = sensorChannels[0]->size();
    playbackFrac = 1.0 / dataLengthInSamps;
    
    if (! sensorChannels.isEmpty())
    {
        windowButtons[1]->setEnabled(true);
        windowButtons[2]->setEnabled(true);
        fileFeedback.setText("Now ready to simulate playback over OSC!");
        trainingData.setNumDimensions(sampleSet.size());
    }

}

void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    transportSource.getNextAudioBlock (bufferToFill);
}

void MainContentComponent::releaseResources()
{
    transportSource.releaseResources();
}

void MainContentComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState (Playing);
        else
            changeState (Stopped);
    }
}
