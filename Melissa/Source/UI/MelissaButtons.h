//
//  MelissaButtons.h
//  Melissa
//
//  Copyright(c) 2020 Masaki Ono
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MelissaDataSource.h"
#include "MelissaModel.h"
#include "MelissaModelListener.h"
#include "MelissaUISettings.h"

class MelissaMenuButton : public Button
{
public:
    MelissaMenuButton() : Button(""), showBudge_(true) { }
    void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        constexpr int lineHeight = 2;
        
        const bool highlighted = shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown;
        g.setColour(MelissaUISettings::getWaveformColour().withAlpha(highlighted ? 1.f : 0.6f));
        
        g.fillRoundedRectangle(0, 4, 26, lineHeight, lineHeight / 2);
        g.fillRoundedRectangle(0, 10, 22, lineHeight, lineHeight / 2);
        g.fillRoundedRectangle(0, 16, 18, lineHeight, lineHeight / 2);
        
        if (showBudge_)
        {
            g.setColour(Colours::red);
            g.fillEllipse(22, 0, 8, 8);
        }
    }
    
    void setBudgeVisibility(bool show) {
        showBudge_ = show;
        repaint();
    }
    
private:
    bool showBudge_;
};

class MelissaAddButton : public Button
{
public:
    MelissaAddButton() : Button("") { }
    void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        constexpr int lineThickness = 2;
        
        const bool highlighed = shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown;
        g.setColour(MelissaUISettings::getMainColour().withAlpha(highlighed ? 1.f : 0.6f));
        
        const int w = getWidth();
        const int h = getHeight();
        const int size = w;
        const int lineLength = size * 2 / 5;
        g.drawRoundedRectangle(1, 1, size - 2, size - 2, (size - 2) / 2, 1);
        g.fillRect((w - lineLength) / 2, (h - lineThickness) / 2, lineLength, lineThickness);
        const int hHalf = (lineLength - lineThickness) / 2;
        g.fillRect((w - lineThickness) / 2, (h - lineLength) / 2, lineThickness, hHalf);
        g.fillRect((w - lineThickness) / 2, (h - lineLength) / 2 + lineThickness + hHalf, lineThickness, hHalf);
    }
};

class MelissaPlayPauseButton : public Button, public MelissaModelListener
{
public:
    MelissaPlayPauseButton(const String& name = "") :
    Button(name), drawPlayIcon_(true)
    {
        MelissaModel::getInstance()->addListener(this);
    }
    
    void playbackStatusChanged(PlaybackStatus status) override
    {
        drawPlayIcon_ = (status != kPlaybackStatus_Playing);
        repaint();
    }
    
private:
    void paintButton (Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        const auto& b = getLocalBounds();
        
        const int w = b.getWidth();
        const int h = b.getHeight();
        const int triW = w * 9.f / 23.f;
        const int triH = h * 3.f / 7.f;
        
        const bool on = shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown;
        //g.setColour(MelissaUISettings::getSubColour());
        //g.fillEllipse(t / 2, t / 2, w - t - 1, h - t - 1);
        
        g.setColour(MelissaUISettings::getTextColour(on ? 1.f : 0.8f));
        if (drawPlayIcon_)
        {
            const int x0 = (w - triW) * 4.f / 7.f;
            const int y0 = (h - triH) / 2;
            
            Path path;
            path.addTriangle (x0, y0, x0, y0 + triH, x0 + triW, h / 2);
            g.fillPath(path);
        }
        else
        {
            const int w0 = w / 10;
            const int l0 = h * 2 / 5;
            
            g.fillRect(w / 2 - w0 * 1.5, (h - l0) / 2, w0, l0);
            g.fillRect(w / 2 + w0 * 0.5, (h - l0) / 2, w0, l0);
        }
    }
    
    bool drawPlayIcon_;
};

class CloseButton : public Button
{
public:
    CloseButton() : Button("")  { }
    ~CloseButton() { }
    
    void paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        const bool highlighted = shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown;
        
        const int size = getWidth();
        g.setColour(Colours::black.withAlpha(0.4f));
        g.fillRoundedRectangle(0, 0, size, size, size / 5);
        
        if (highlighted)
        {
            g.setColour(MelissaUISettings::getMainColour().withAlpha(0.2f));
            g.fillRoundedRectangle(0, 0, size, size, size / 6);
        }
        
        const int margin = 4;
        g.setColour(Colours::white.withAlpha(0.8f));
        g.drawLine(margin, margin, size - margin, size - margin);
        g.drawLine(margin, size - margin, size - margin, margin);
        
    }
};

class BackgroundButton : public Button
{
public:
    BackgroundButton() : Button("")  { }
    ~BackgroundButton() { }
    void paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override { }
};

class MelissaRoundButton : public Button
{
public:
    MelissaRoundButton(const String& title) : Button(""), title_(title)
    {
        // default
        normalColour_ = MelissaUISettings::getAccentColour(0.6f);
        highlightedColour_ = downColour_ = MelissaUISettings::getAccentColour(1.f);
    }
    
    void paintButton(Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        Colour colour = normalColour_;
        if (shouldDrawButtonAsDown)
        {
            colour = downColour_;
        }
        else if (shouldDrawButtonAsHighlighted)
        {
            colour = highlightedColour_;
        }
        
        g.setColour(colour);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), getHeight() / 2);
        
        g.setColour(MelissaUISettings::getTextColour());
        g.setFont(font_);
        g.drawText(title_, 0, 0, getWidth(), getHeight(), Justification::centred);
    }

    void setFont(Font font)
    {
        font_ = font;
        repaint();
    }
    
    void setColour(Colour normalColour, Colour highlightedColour, Colour downColour)
    {
        normalColour_ = normalColour;
        highlightedColour_ = highlightedColour;
        downColour_ = downColour;
        repaint();
    }
    
private:
    String title_;
    Font font_;
    Colour normalColour_, highlightedColour_, downColour_;
};

class MelissaAudioDeviceButton : public Button
{
public:
    MelissaAudioDeviceButton() : Button(""), name_()
    {
        normal_ = Drawable::createFromImageData(BinaryData::speaker_svg, BinaryData::speaker_svgSize);
        normal_->replaceColour(Colours::white, MelissaUISettings::getTextColour(0.8f));
        
        highlighted_ = Drawable::createFromImageData(BinaryData::speaker_svg, BinaryData::speaker_svgSize);
        highlighted_->replaceColour(Colours::white, MelissaUISettings::getTextColour());
    }
    
    ~MelissaAudioDeviceButton() { }
    
    void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        if (shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown)
        {
            highlighted_->drawAt(g, 10, 10, 1.f);
        }
        else
        {
            normal_->drawAt(g, 10, 10, 1.f);
        }
        
        g.setColour(MelissaUISettings::getTextColour((shouldDrawButtonAsHighlighted || shouldDrawButtonAsDown) ? 1.f : 0.8f));
        g.setFont(MelissaDataSource::getInstance()->getFont(MelissaDataSource::Global::kFontSize_Main));
        g.drawText(name_, 30, 0, getWidth() - 30, getHeight(), Justification::centred);
    }
    
    void setAudioDeviceName(const String& name)
    {
        name_ = name;
        repaint();
    }
    
private:
    String name_;
    std::unique_ptr<Drawable> normal_;
    std::unique_ptr<Drawable> highlighted_;
};
