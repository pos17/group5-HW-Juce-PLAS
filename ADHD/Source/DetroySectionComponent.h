/*
  ==============================================================================

    DetroySectionComponent.h
    Created: 19 Apr 2022 4:51:06pm
    Author:  stefa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class DetroySectionComponent  : public juce::Component
{
public:
    DetroySectionComponent();
    ~DetroySectionComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DetroySectionComponent)
};
