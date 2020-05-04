//
//  MainComponent.h
//  Melissa
//
//  Copyright(c) 2020 Masaki Ono
//

#pragma once

#include <numeric>
#include "../JuceLibraryCode/JuceHeader.h"
#include "MelissaAudioEngine.h"
#include "MelissaBottomControlComponent.h"
#include "MelissaBPMDetector.h"
#include "MelissaButtons.h"
#include "MelissaDataSource.h"
#include "MelissaFileListBox.h"
#include "MelissaHost.h"
#include "MelissaIncDecButton.h"
#include "MelissaLookAndFeel.h"
#include "MelissaMIDIControlManager.h"
#include "MelissaModalDialog.h"
#include "MelissaModel.h"
#include "MelissaPlaylistComponent.h"
#include "MelissaPracticeTableListBox.h"
#include "MelissaPreferencesComponent.h"
#include "MelissaScrollLabel.h"
#include "MelissaToHeadButton.h"
#include "MelissaTutorialComponent.h"
#include "MelissaUpdateChecker.h"
#include "MelissaUtility.h"
#include "MelissaWaveformControlComponent.h"

enum FileChooserTab
{
    kFileChooserTab_Browse,
    kFileChooserTab_Playlist,
    kFileChooserTab_History,
    kNumOfFileChooserTabs
};

enum PracticeMemoTab
{
    kPracticeMemoTab_Practice,
    kPracticeMemoTab_Memo
};

class MelissaSectionTitleComponent : public Component
{
public:
    MelissaSectionTitleComponent(const String& title, float lineRatio) :
    lineRatio_(lineRatio)
    {
        label_ = std::make_unique<Label>();
        label_->setColour(Label::textColourId, Colours::white.withAlpha(0.6f));
        label_->setText(title, dontSendNotification);
        label_->setJustificationType(Justification::centred);
        label_->setFont(Font(MelissaUISettings::getFontSizeMain()));
        addAndMakeVisible(label_.get());
        
        labelWidth_ = label_->getFont().getStringWidth(title);
    }
    
    void resized() override
    {
        label_->setBounds(getLocalBounds());
    }
    
    void paint(Graphics& g) override
    {
        labelWidth_ = label_->getFont().getStringWidth(label_->getText());
        constexpr int lineHeight = 1;
        g.setColour(Colours::white.withAlpha(0.4f));
        
        const int lineWidth = getWidth() * lineRatio_;
        g.fillRect(0, (getHeight() - lineHeight) / 2, lineWidth, lineHeight);
        g.fillRect(getWidth() - lineWidth, (getHeight() - lineHeight) / 2, lineWidth, lineHeight);
    }

private:
    std::unique_ptr<Label> label_;
    int labelWidth_;
    float lineRatio_;
};

class MainComponent   : public AudioAppComponent,
                        public FileDragAndDropTarget,
                        public FileBrowserListener,
                        public KeyListener,
                        public MelissaDataSourceListener,
                        public MelissaHost,
                        public MelissaModelListener,
                        public MenuBarModel,
                        public MidiInputCallback,
                        public Timer,
                        public Thread,
                        public Thread::Listener
{
public:
    MainComponent();
    ~MainComponent();
    
    void createUI();
    void showFileChooser();
    
    // AudioAppComponent
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void paint(Graphics& g) override;
    void resized() override;
    
    // FileDragAndDropTarget
    bool isInterestedInFileDrag(const StringArray &files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    
    // FileBrowserListener
    void selectionChanged() override {};
    void fileClicked(const File& file, const MouseEvent& e) override {}
    void fileDoubleClicked(const File& file) override;
    void browserRootChanged(const File& newRoot) override;
    
    // KeyListener
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    
    // Melissa
    void closeTutorial() override;
    
    // MelissaDataSourceListener
    void songChanged(const String& filePath, size_t bufferLength, int32_t sampleRate) override;
    void fileLoadStatusChanged(FileLoadStatus status, const String& filePath) override;
    
    // MenuBarModel
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    
    // MidiInputCallback
    void handleIncomingMidiMessage(MidiInput *source, const MidiMessage &message) override;

    // Thread
    void run() override;
    
    // Thread
    void exitSignalSent() override;
    
    // Timer
    void timerCallback() override;
    
    void updateFileChooserTab(FileChooserTab tab);
    void updatePracticeMemo(PracticeMemoTab tab);
    
    void toHead();
    void resetLoop();
    void addToPracticeList(String name);
    void saveMemo();
    
    var getSongSetting(String fileName);
    void showPreferencesDialog();
    void showAboutDialog();
    
    void showTutorial();
    void showUpdateDialog(bool showIfThereIsNoUpdate = false);

private:
    std::unique_ptr<MelissaAudioEngine> audioEngine_;
    MelissaModel* model_;
    MelissaDataSource* dataSource_;
    std::unique_ptr<MelissaBPMDetector> bpmDetector_;
    
    std::shared_ptr<AudioSampleBuffer> audioSampleBuf_;
    
    std::unique_ptr<MelissaMenuButton> menuButton_;
    
    std::unique_ptr<PopupMenu> extraAppleMenuItems_;
    std::unique_ptr<MenuBarComponent> menuBar_;
    
    std::unique_ptr<MelissaWaveformControlComponent> waveformComponent_;
    std::unique_ptr<Label> controlComponent_;
    std::unique_ptr<MelissaBottomControlComponent> bottomComponent_;
    
    std::unique_ptr<MelissaPlayPauseButton> playPauseButton_;
    std::unique_ptr<MelissaToHeadButton> toHeadButton_;
    
    std::unique_ptr<Label> timeLabel_;
    std::unique_ptr<MelissaScrollLabel> fileNameLabel_;

    std::unique_ptr<ToggleButton> metronomeOnOffButton_;
    std::unique_ptr<MelissaIncDecButton> bpmButton_;
    std::unique_ptr<MelissaIncDecButton> beatPositionButton_;
    std::unique_ptr<TextButton> tapButton_;
    std::unique_ptr<TextButton> analyzeButton_;
    
    std::unique_ptr<Slider> volumeSlider_;
    
    std::unique_ptr<MelissaIncDecButton> aButton_;
    std::unique_ptr<MelissaIncDecButton> bButton_;
    std::unique_ptr<TextButton> resetButton_;
    
    std::unique_ptr<MelissaIncDecButton> speedButton_;

#if defined(ENABLE_SPEEDTRAINER)
    std::unique_ptr<MelissaIncDecButton> speedIncPerButton_;
    std::unique_ptr<MelissaIncDecButton> speedIncValueButton_;
    std::unique_ptr<MelissaIncDecButton> speedIncMaxButton_;
#endif
    
    std::unique_ptr<MelissaIncDecButton> pitchButton_;
    
    std::unique_ptr<ToggleButton> browseToggleButton_;
    std::unique_ptr<ToggleButton> playlistToggleButton_;
    std::unique_ptr<ToggleButton> historyToggleButton_;
    std::unique_ptr<WildcardFileFilter> wildCardFilter_;
    std::unique_ptr<FileBrowserComponent> fileBrowserComponent_;
    std::unique_ptr<MelissaFileListBox> historyTable_;
    
    std::unique_ptr<ToggleButton> practiceListToggleButton_;
    std::unique_ptr<ToggleButton> memoToggleButton_;
    std::unique_ptr<TextEditor> memoTextEditor_;
    std::unique_ptr<MelissaAddButton> addToListButton_;
    std::unique_ptr<MelissaPracticeTableListBox> practiceTable_;
    
    std::unique_ptr<ComboBox> oututModeComboBox_;
    
    std::unique_ptr<MelissaPreferencesComponent> preferencesComponent_;
    std::unique_ptr<MelissaPlaylistComponent> playlistComponent_;
    std::unique_ptr<MelissaModalDialog> modalDialog_;
    
    std::unique_ptr<FileChooser> fileChooser_;
    
    std::unique_ptr<MelissaTutorialComponent> tutorialComponent_;
    
    enum
    {
        kLabel_MetronomeSw,
        kLabel_MetronomeBpm,
        kLabel_MetronomeOffset,
        kLabel_Volume,
        kLabel_Pitch,
        kLabel_OutputMode,
        
        kLabel_ATime,
        kLabel_BTime,
        
        kLabel_Speed,
#if defined(ENABLE_SPEEDTRAINER)
        kLabel_SpeedPlus,
        kLabel_SpeedPer,
        kLabel_SpeedMax,
#endif
        kNumOfLabels
    };
    std::unique_ptr<Label> labels_[kNumOfLabels];
    
    enum
    {
        kSectionTitle_Settings,
        kSectionTitle_Loop,
#if defined(ENABLE_SPEEDTRAINER)
        kSectionTitle_Speed,
#endif
        kSectionTitle_Metronome,
        kNumOfSectionTitles
    };
    std::unique_ptr<MelissaSectionTitleComponent> sectionTitles_[kNumOfSectionTitles];
    
    MelissaLookAndFeel lookAndFeel_;
    MelissaLookAndFeel_Tab lookAndFeelTab_;
    MelissaLookAndFeel_Memo lookAndFeelMemo_;
    
    String fileName_, fileFullPath_;
    
    File settingsDir_, settingsFile_;
    
    bool shouldExit_;
    
    MelissaMIDIControlManager midiControlManager_;
    
    std::unique_ptr<TooltipWindow> tooltipWindow_;
    
    void arrangeEvenly(const Rectangle<int> bounds, const std::vector<std::vector<Component*>>& components_, float widthRatio = 1.f);
    
    // MelissaModelListener
    void volumeChanged(float volume) override;
    void pitchChanged(int semitone) override;
    void speedChanged(int speed) override;
    void loopPosChanged(float aTimeMSec, float aRatio, float bTimeMSec, float bRatio) override;
    void bpmChanged(float bpm) override;
    void beatPositionChanged(float beatPositionMSec) override;
    void outputModeChanged(OutputMode outputMode) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
