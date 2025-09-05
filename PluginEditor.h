#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// Custom component to group a knob and its label
class KnobWithLabel : public juce::Component
{
public:
    KnobWithLabel(juce::Slider& slider, juce::Label& label)
        : knob(slider), label(label)
    {
        addAndMakeVisible(knob);
        addAndMakeVisible(label);
    }

    void resized() override
    {
        const float phi = 1.61803398875f;
        const int knobWidth = 80; // 2x original 40
        const int knobHeight = static_cast<int>(knobWidth / phi); // ≈ 49
        const int labelHeight = static_cast<int>(knobHeight / phi); // ≈ 30
        const int labelSpacing = 10; // 2x original 5

        knob.setBounds(0, 0, knobWidth, knobHeight);
        label.setBounds(0, knobHeight + labelSpacing, knobWidth + 40, labelHeight); // Wider to prevent truncation
    }

private:
    juce::Slider& knob;
    juce::Label& label;
};

// Plugin editor class
class DrumRAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DrumRAudioProcessorEditor(DrumDesignerAudioProcessor&);
    ~DrumRAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    // Custom LookAndFeel for metallic knobs
    class MetallicLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
            float sliderPos, float startAngle, float endAngle, juce::Slider&) override;
    };

private:
    DrumDesignerAudioProcessor& proc;
    MetallicLookAndFeel lookAndFeel;

    // Sliders
    juce::Slider inputTrimSlider, modeSlider, hpfHzSlider, lpfHzSlider, attAmtSlider, susAmtSlider,
        compThresholdSlider, compRatioSlider, compAttackSlider, compReleaseSlider, compKneeSlider,
        scHpfHzSlider, compMixSlider, satTypeSlider, satDriveSlider, satBiasSlider, satAsymSlider,
        osFactorSlider, satMixSlider, tiltSlider, bodyHzSlider, bodyGainSlider, gateThreshSlider,
        gateRatioSlider, gateAttSlider, gateRelSlider, gateHystSlider, clipCeilSlider, limLookSlider,
        dryWetSlider, autoGainSlider, outputTrimSlider;

    // Labels
    juce::Label inputTrimLabel, modeLabel, hpfHzLabel, lpfHzLabel, attAmtLabel, susAmtLabel,
        compThresholdLabel, compRatioLabel, compAttackLabel, compReleaseLabel, compKneeLabel,
        scHpfHzLabel, compMixLabel, satTypeLabel, satDriveLabel, satBiasLabel, satAsymLabel,
        osFactorLabel, satMixLabel, tiltLabel, bodyHzLabel, bodyGainLabel, gateThreshLabel,
        gateRatioLabel, gateAttLabel, gateRelLabel, gateHystLabel, clipCeilLabel, limLookLabel,
        dryWetLabel, autoGainLabel, outputTrimLabel;

    // KnobWithLabel containers
    std::unique_ptr<KnobWithLabel> inputTrimContainer, modeContainer, hpfHzContainer, lpfHzContainer,
        attAmtContainer, susAmtContainer, compThresholdContainer, compRatioContainer, compAttackContainer,
        compReleaseContainer, compKneeContainer, scHpfHzContainer, compMixContainer, satTypeContainer,
        satDriveContainer, satBiasContainer, satAsymContainer, osFactorContainer, satMixContainer,
        tiltContainer, bodyHzContainer, bodyGainContainer, gateThreshContainer, gateRatioContainer,
        gateAttContainer, gateRelContainer, gateHystContainer, clipCeilContainer, limLookContainer,
        dryWetContainer, autoGainContainer, outputTrimContainer;

    // Parameter attachments
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> inputTrimAttachment, modeAttachment, hpfHzAttachment,
        lpfHzAttachment, attAmtAttachment, susAmtAttachment, compThresholdAttachment,
        compRatioAttachment, compAttackAttachment, compReleaseAttachment, compKneeAttachment,
        scHpfHzAttachment, compMixAttachment, satTypeAttachment, satDriveAttachment,
        satBiasAttachment, satAsymAttachment, osFactorAttachment, satMixAttachment,
        tiltAttachment, bodyHzAttachment, bodyGainAttachment, gateThreshAttachment,
        gateRatioAttachment, gateAttAttachment, gateRelAttachment, gateHystAttachment,
        clipCeilAttachment, limLookAttachment, dryWetAttachment, autoGainAttachment,
        outputTrimAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumRAudioProcessorEditor)
};