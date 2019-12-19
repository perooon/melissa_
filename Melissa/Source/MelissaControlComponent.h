#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MelissaControlComponent : public Component
{
public:
    MelissaControlComponent()
    {
        setOpaque(false);
    }
    
    ~MelissaControlComponent() {}
    
    void paint(Graphics& g) override
    {
        constexpr int gradMargin = 20;
        const int w = getWidth();
        const int h = getHeight();
        
        g.setGradientFill(ColourGradient(Colour(0x00000000), w / 2, 0.f, Colour(0x16000000), w / 2, gradMargin, false));
        g.fillRect(0, 0, w, gradMargin);
        
        g.setColour(Colour::fromFloatRGBA(1.f, 1.f, 1.f, 0.16f));
        g.fillRect(0, gradMargin, w, h - 2 * gradMargin);
        
        g.setGradientFill(ColourGradient(Colour(0x16000000), w / 2, h - gradMargin, Colour(0x00000000), w / 2, h, false));
        g.fillRect(0, h - gradMargin, w, gradMargin);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MelissaControlComponent);
};
