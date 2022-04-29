/*
  ==============================================================================

    PlasLookAndFeel.h
    Created: 12 Apr 2022 2:12:55pm
    Author:  Pox17

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PLASLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASLookAndFeel();
    void setKnobStripN(juce::Image src);
    void setKnobStripD(juce::Image src);
    void setFrames(int fr);
    void setDestroy(bool dest);
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
private:
    juce::Image knobStripN;
    juce::Image knobStripD;
    juce::Image ButtonN;
    juce::Image buttonD;
    int frames;
    bool destroy; 
};

class PLASBigKnobLookFeel : public PLASLookAndFeel {
    public:
        PLASBigKnobLookFeel();
};
class PLASLittleKnobLookFeel : public PLASLookAndFeel {
    public:
        PLASLittleKnobLookFeel();
};


class PLASDestroyButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASDestroyButtonLookAndFeel();
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) 	 override;
private:
    juce::Image buttonOn;
    juce::Image buttonOff;
};

class PLASButtonLookAndFeel : public juce::LookAndFeel_V4 {
public:
    PLASButtonLookAndFeel(juce::Image abuttonOnN,
                          juce::Image abuttonOffN,
                          juce::Image abuttonOnD,
                          juce::Image abuttonOffD,
                          bool destroy);
    void drawToggleButton(juce::Graphics&, juce::ToggleButton&, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)      override;
    void setDestroy(bool val);
private:
    juce::Image buttonOnN;
    juce::Image buttonOffN;
    juce::Image buttonOnD;
    juce::Image buttonOffD;
    bool destroy;
};

class PLASButLinkLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButLinkLookAndFeel();
};
class PLASButOnLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButOnLookAndFeel();
};

class PLASButLPLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButLPLookAndFeel();
};

class PLASButBPLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButBPLookAndFeel();
};
class PLASButHPLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButHPLookAndFeel();
};
class PLASButMSLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButMSLookAndFeel();
};
class PLASButLRLookAndFeel: public PLASButtonLookAndFeel {
public:
    PLASButLRLookAndFeel();
};
