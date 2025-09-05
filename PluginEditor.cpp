#include "PluginEditor.h"

DrumRAudioProcessorEditor::DrumRAudioProcessorEditor(DrumDesignerAudioProcessor& p)
    : AudioProcessorEditor(&p), proc(p)
{
    setLookAndFeel(&lookAndFeel);

    // Enable resizable window
    setResizable(true, true);
    getConstrainer()->setMinimumSize(1200, 1200);
    getConstrainer()->setMaximumSize(2000, 2000);

    // Initialize sliders with custom text formatting
    auto initSlider = [&](juce::Slider& slider, juce::Label& label, const juce::String& labelText)
        {
            addAndMakeVisible(slider);
            slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 120, 28); // 2x original 60x14
            addAndMakeVisible(label);
            label.setText(labelText, juce::dontSendNotification);
            label.setJustificationType(juce::Justification::centred);
            label.setColour(juce::Label::textColourId, juce::Colour(200, 200, 200));
            label.setFont(juce::FontOptions(20.0f)); // 2x original 10.0f
        };

    auto initChoiceSlider = [&](juce::Slider& slider, juce::Label& label, const juce::String& labelText)
        {
            initSlider(slider, label, labelText);
            slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 120, 28);
        };

    // Initialize all sliders, labels, and KnobWithLabel containers
    initSlider(inputTrimSlider, inputTrimLabel, "Input (dB)");
    inputTrimContainer = std::make_unique<KnobWithLabel>(inputTrimSlider, inputTrimLabel);
    addAndMakeVisible(*inputTrimContainer);
    initChoiceSlider(modeSlider, modeLabel, "Mode");
    modeContainer = std::make_unique<KnobWithLabel>(modeSlider, modeLabel);
    addAndMakeVisible(*modeContainer);
    initSlider(hpfHzSlider, hpfHzLabel, "HPF (Hz)");
    hpfHzContainer = std::make_unique<KnobWithLabel>(hpfHzSlider, hpfHzLabel);
    addAndMakeVisible(*hpfHzContainer);
    initSlider(lpfHzSlider, lpfHzLabel, "LPF (Hz)");
    lpfHzContainer = std::make_unique<KnobWithLabel>(lpfHzSlider, lpfHzLabel);
    addAndMakeVisible(*lpfHzContainer);
    initSlider(attAmtSlider, attAmtLabel, "Attack (dB)");
    attAmtContainer = std::make_unique<KnobWithLabel>(attAmtSlider, attAmtLabel);
    addAndMakeVisible(*attAmtContainer);
    initSlider(susAmtSlider, susAmtLabel, "Sustain (dB)");
    susAmtContainer = std::make_unique<KnobWithLabel>(susAmtSlider, susAmtLabel);
    addAndMakeVisible(*susAmtContainer);
    initSlider(compThresholdSlider, compThresholdLabel, "Comp Thresh (dB)");
    compThresholdContainer = std::make_unique<KnobWithLabel>(compThresholdSlider, compThresholdLabel);
    addAndMakeVisible(*compThresholdContainer);
    initSlider(compRatioSlider, compRatioLabel, "Comp Ratio");
    compRatioContainer = std::make_unique<KnobWithLabel>(compRatioSlider, compRatioLabel);
    addAndMakeVisible(*compRatioContainer);
    initSlider(compAttackSlider, compAttackLabel, "Comp Att (ms)");
    compAttackContainer = std::make_unique<KnobWithLabel>(compAttackSlider, compAttackLabel);
    addAndMakeVisible(*compAttackContainer);
    initSlider(compReleaseSlider, compReleaseLabel, "Comp Rel (ms)");
    compReleaseContainer = std::make_unique<KnobWithLabel>(compReleaseSlider, compReleaseLabel);
    addAndMakeVisible(*compReleaseContainer);
    initSlider(compKneeSlider, compKneeLabel, "Comp Knee (dB)");
    compKneeContainer = std::make_unique<KnobWithLabel>(compKneeSlider, compKneeLabel);
    addAndMakeVisible(*compKneeContainer);
    initSlider(scHpfHzSlider, scHpfHzLabel, "SC HPF (Hz)");
    scHpfHzContainer = std::make_unique<KnobWithLabel>(scHpfHzSlider, scHpfHzLabel);
    addAndMakeVisible(*scHpfHzContainer);
    initSlider(compMixSlider, compMixLabel, "Comp Mix (%)");
    compMixContainer = std::make_unique<KnobWithLabel>(compMixSlider, compMixLabel);
    addAndMakeVisible(*compMixContainer);
    initChoiceSlider(satTypeSlider, satTypeLabel, "Sat Type");
    satTypeContainer = std::make_unique<KnobWithLabel>(satTypeSlider, satTypeLabel);
    addAndMakeVisible(*satTypeContainer);
    initSlider(satDriveSlider, satDriveLabel, "Drive (dB)");
    satDriveContainer = std::make_unique<KnobWithLabel>(satDriveSlider, satDriveLabel);
    addAndMakeVisible(*satDriveContainer);
    initSlider(satBiasSlider, satBiasLabel, "Bias (dB)");
    satBiasContainer = std::make_unique<KnobWithLabel>(satBiasSlider, satBiasLabel);
    addAndMakeVisible(*satBiasContainer);
    initSlider(satAsymSlider, satAsymLabel, "Asymmetry (%)");
    satAsymContainer = std::make_unique<KnobWithLabel>(satAsymSlider, satAsymLabel);
    addAndMakeVisible(*satAsymContainer);
    initChoiceSlider(osFactorSlider, osFactorLabel, "Oversampling");
    osFactorContainer = std::make_unique<KnobWithLabel>(osFactorSlider, osFactorLabel);
    addAndMakeVisible(*osFactorContainer);
    initSlider(satMixSlider, satMixLabel, "Sat Mix (%)");
    satMixContainer = std::make_unique<KnobWithLabel>(satMixSlider, satMixLabel);
    addAndMakeVisible(*satMixContainer);
    initSlider(tiltSlider, tiltLabel, "Tilt (dB)");
    tiltContainer = std::make_unique<KnobWithLabel>(tiltSlider, tiltLabel);
    addAndMakeVisible(*tiltContainer);
    initSlider(bodyHzSlider, bodyHzLabel, "Body (Hz)");
    bodyHzContainer = std::make_unique<KnobWithLabel>(bodyHzSlider, bodyHzLabel);
    addAndMakeVisible(*bodyHzContainer);
    initSlider(bodyGainSlider, bodyGainLabel, "Body Gain (dB)");
    bodyGainContainer = std::make_unique<KnobWithLabel>(bodyGainSlider, bodyGainLabel);
    addAndMakeVisible(*bodyGainContainer);
    initSlider(gateThreshSlider, gateThreshLabel, "Gate Thresh (dB)");
    gateThreshContainer = std::make_unique<KnobWithLabel>(gateThreshSlider, gateThreshLabel);
    addAndMakeVisible(*gateThreshContainer);
    initSlider(gateRatioSlider, gateRatioLabel, "Gate Ratio");
    gateRatioContainer = std::make_unique<KnobWithLabel>(gateRatioSlider, gateRatioLabel);
    addAndMakeVisible(*gateRatioContainer);
    initSlider(gateAttSlider, gateAttLabel, "Gate Att (ms)");
    gateAttContainer = std::make_unique<KnobWithLabel>(gateAttSlider, gateAttLabel);
    addAndMakeVisible(*gateAttContainer);
    initSlider(gateRelSlider, gateRelLabel, "Gate Rel (ms)");
    gateRelContainer = std::make_unique<KnobWithLabel>(gateRelSlider, gateRelLabel);
    addAndMakeVisible(*gateRelContainer);
    initSlider(gateHystSlider, gateHystLabel, "Gate Hyst (dB)");
    gateHystContainer = std::make_unique<KnobWithLabel>(gateHystSlider, gateHystLabel);
    addAndMakeVisible(*gateHystContainer);
    initSlider(clipCeilSlider, clipCeilLabel, "Ceiling (dB)");
    clipCeilContainer = std::make_unique<KnobWithLabel>(clipCeilSlider, clipCeilLabel);
    addAndMakeVisible(*clipCeilContainer);
    initSlider(limLookSlider, limLookLabel, "Lookahead (ms)");
    limLookContainer = std::make_unique<KnobWithLabel>(limLookSlider, limLookLabel);
    addAndMakeVisible(*limLookContainer);
    initSlider(dryWetSlider, dryWetLabel, "Dry/Wet (%)");
    dryWetContainer = std::make_unique<KnobWithLabel>(dryWetSlider, dryWetLabel);
    addAndMakeVisible(*dryWetContainer);
    initSlider(autoGainSlider, autoGainLabel, "Auto Gain");
    autoGainContainer = std::make_unique<KnobWithLabel>(autoGainSlider, autoGainLabel);
    addAndMakeVisible(*autoGainContainer);
    initSlider(outputTrimSlider, outputTrimLabel, "Output (dB)");
    outputTrimContainer = std::make_unique<KnobWithLabel>(outputTrimSlider, outputTrimLabel);
    addAndMakeVisible(*outputTrimContainer);

    // Attach sliders to parameters
    inputTrimAttachment = std::make_unique<SliderAttachment>(proc.apvts, "inputTrim", inputTrimSlider);
    modeAttachment = std::make_unique<SliderAttachment>(proc.apvts, "mode", modeSlider);
    hpfHzAttachment = std::make_unique<SliderAttachment>(proc.apvts, "hpfHz", hpfHzSlider);
    lpfHzAttachment = std::make_unique<SliderAttachment>(proc.apvts, "lpfHz", lpfHzSlider);
    attAmtAttachment = std::make_unique<SliderAttachment>(proc.apvts, "attAmt", attAmtSlider);
    susAmtAttachment = std::make_unique<SliderAttachment>(proc.apvts, "susAmt", susAmtSlider);
    compThresholdAttachment = std::make_unique<SliderAttachment>(proc.apvts, "compThreshold", compThresholdSlider);
    compRatioAttachment = std::make_unique<SliderAttachment>(proc.apvts, "compRatio", compRatioSlider);
    compAttackAttachment = std::make_unique<SliderAttachment>(proc.apvts, "compAttack", compAttackSlider);
    compReleaseAttachment = std::make_unique<SliderAttachment>(proc.apvts, "compRelease", compReleaseSlider);
    compKneeAttachment = std::make_unique<SliderAttachment>(proc.apvts, "compKnee", compKneeSlider);
    scHpfHzAttachment = std::make_unique<SliderAttachment>(proc.apvts, "scHpfHz", scHpfHzSlider);
    compMixAttachment = std::make_unique<SliderAttachment>(proc.apvts, "compMix", compMixSlider);
    satTypeAttachment = std::make_unique<SliderAttachment>(proc.apvts, "satType", satTypeSlider);
    satDriveAttachment = std::make_unique<SliderAttachment>(proc.apvts, "satDrive", satDriveSlider);
    satBiasAttachment = std::make_unique<SliderAttachment>(proc.apvts, "satBias", satBiasSlider);
    satAsymAttachment = std::make_unique<SliderAttachment>(proc.apvts, "satAsym", satAsymSlider);
    osFactorAttachment = std::make_unique<SliderAttachment>(proc.apvts, "osFactor", osFactorSlider);
    satMixAttachment = std::make_unique<SliderAttachment>(proc.apvts, "satMix", satMixSlider);
    tiltAttachment = std::make_unique<SliderAttachment>(proc.apvts, "tilt", tiltSlider);
    bodyHzAttachment = std::make_unique<SliderAttachment>(proc.apvts, "bodyHz", bodyHzSlider);
    bodyGainAttachment = std::make_unique<SliderAttachment>(proc.apvts, "bodyGain", bodyGainSlider);
    gateThreshAttachment = std::make_unique<SliderAttachment>(proc.apvts, "gateThresh", gateThreshSlider);
    gateRatioAttachment = std::make_unique<SliderAttachment>(proc.apvts, "gateRatio", gateRatioSlider);
    gateAttAttachment = std::make_unique<SliderAttachment>(proc.apvts, "gateAtt", gateAttSlider);
    gateRelAttachment = std::make_unique<SliderAttachment>(proc.apvts, "gateRel", gateRelSlider);
    gateHystAttachment = std::make_unique<SliderAttachment>(proc.apvts, "gateHyst", gateHystSlider);
    clipCeilAttachment = std::make_unique<SliderAttachment>(proc.apvts, "clipCeil", clipCeilSlider);
    limLookAttachment = std::make_unique<SliderAttachment>(proc.apvts, "limLook", limLookSlider);
    dryWetAttachment = std::make_unique<SliderAttachment>(proc.apvts, "dryWet", dryWetSlider);
    autoGainAttachment = std::make_unique<SliderAttachment>(proc.apvts, "autoGain", autoGainSlider);
    outputTrimAttachment = std::make_unique<SliderAttachment>(proc.apvts, "outputTrim", outputTrimSlider);

    // Set text value suffix for appropriate units
    inputTrimSlider.setTextValueSuffix(" dB");
    hpfHzSlider.setTextValueSuffix(" Hz");
    lpfHzSlider.setTextValueSuffix(" Hz");
    attAmtSlider.setTextValueSuffix(" dB");
    susAmtSlider.setTextValueSuffix(" dB");
    compThresholdSlider.setTextValueSuffix(" dB");
    compRatioSlider.setTextValueSuffix(":1");
    compAttackSlider.setTextValueSuffix(" ms");
    compReleaseSlider.setTextValueSuffix(" ms");
    compKneeSlider.setTextValueSuffix(" dB");
    scHpfHzSlider.setTextValueSuffix(" Hz");
    compMixSlider.setTextValueSuffix(" %");
    satDriveSlider.setTextValueSuffix(" dB");
    satBiasSlider.setTextValueSuffix(" dB");
    satAsymSlider.setTextValueSuffix(" %");
    satMixSlider.setTextValueSuffix(" %");
    tiltSlider.setTextValueSuffix(" dB");
    bodyHzSlider.setTextValueSuffix(" Hz");
    bodyGainSlider.setTextValueSuffix(" dB");
    gateThreshSlider.setTextValueSuffix(" dB");
    gateRatioSlider.setTextValueSuffix(":1");
    gateAttSlider.setTextValueSuffix(" ms");
    gateRelSlider.setTextValueSuffix(" ms");
    gateHystSlider.setTextValueSuffix(" dB");
    clipCeilSlider.setTextValueSuffix(" dB");
    limLookSlider.setTextValueSuffix(" ms");
    dryWetSlider.setTextValueSuffix(" %");
    autoGainSlider.setTextValueSuffix(""); // On/Off
    outputTrimSlider.setTextValueSuffix(" dB");

    // Custom text formatting for Hz (whole numbers)
    auto formatHz = [](double value) { return juce::String((int)value); };
    hpfHzSlider.textFromValueFunction = formatHz;
    lpfHzSlider.textFromValueFunction = formatHz;
    scHpfHzSlider.textFromValueFunction = formatHz;
    bodyHzSlider.textFromValueFunction = formatHz;

    // Custom text formatting for ms (one decimal place)
    auto formatMs = [](double value) { return juce::String(value, 1); };
    compAttackSlider.textFromValueFunction = formatMs;
    compReleaseSlider.textFromValueFunction = formatMs;
    gateAttSlider.textFromValueFunction = formatMs;
    gateRelSlider.textFromValueFunction = formatMs;
    limLookSlider.textFromValueFunction = formatMs;

    // Custom text formatting for % (as percentage, whole numbers)
    auto formatPercent = [](double value) { return juce::String((int)(value * 100.0)); };
    compMixSlider.textFromValueFunction = formatPercent;
    satMixSlider.textFromValueFunction = formatPercent;
    satAsymSlider.textFromValueFunction = formatPercent;
    dryWetSlider.textFromValueFunction = formatPercent;

    // Custom text for choice parameters
    modeSlider.textFromValueFunction = [](double value)
        {
            int idx = (int)value;
            juce::StringArray modes = { "Kick", "Snare", "Tom", "Hat", "Perc", "Bus" };
            return modes[idx];
        };
    satTypeSlider.textFromValueFunction = [](double value)
        {
            int idx = (int)value;
            juce::StringArray types = { "Tanh", "Diode", "Tape" };
            return types[idx];
        };
    osFactorSlider.textFromValueFunction = [](double value)
        {
            int idx = (int)value;
            juce::StringArray factors = { "1x", "2x", "4x" };
            return factors[idx];
        };
    autoGainSlider.textFromValueFunction = [](double value)
        {
            return value > 0.5 ? "On" : "Off";
        };

    // Set initial editor size
    setSize(1110, 1110); // Adjustable later
}

DrumRAudioProcessorEditor::~DrumRAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void DrumRAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
    g.fillAll(juce::Colour(0xFF2A2A2A)); // Dark grey background
    g.setColour(juce::Colour(0xFF00A8CC)); // Teal accent for title
    g.setFont(juce::FontOptions(36.0f)); // Title font size
    g.drawText("DrumR", 30, 20, getWidth() - 60, 50, juce::Justification::centred); // Title at top-left

    // Section headers aligned with rows, spaced by title font size
    g.setColour(juce::Colour(0xFFCCCCCC)); // Light grey for text
    g.setFont(juce::FontOptions(24.0f)); // Header font size
    const int titleFontSize = 36; // Matches title font size
    const int headerHeight = 24; // Matches resized()
    const int cellHeight = 120; // Knob height (~49) + label height (~30) + spacing (10) + text box (~28)
    const int rowSpacing = titleFontSize; // 36px, matches title font size
    const int headerToKnobSpacing = titleFontSize; // 36px
    const int startY = 20 + titleFontSize + rowSpacing; // 20 + 36 + 36 = 92

    // Align headers just above each row
    g.drawText("Input/Output", 30, startY - headerHeight - headerToKnobSpacing, 300, headerHeight, juce::Justification::centredLeft); // Row 1
    g.drawText("Filters/Transient", 30, startY + cellHeight + rowSpacing - headerHeight - headerToKnobSpacing, 300, headerHeight, juce::Justification::centredLeft); // Row 2
    g.drawText("Compressor", 30, startY + 2 * (cellHeight + rowSpacing) - headerHeight - headerToKnobSpacing, 300, headerHeight, juce::Justification::centredLeft); // Row 3
    g.drawText("Saturation", 30, startY + 3 * (cellHeight + rowSpacing) - headerHeight - headerToKnobSpacing, 300, headerHeight, juce::Justification::centredLeft); // Row 4
    g.drawText("EQ/Clipper", 30, startY + 4 * (cellHeight + rowSpacing) - headerHeight - headerToKnobSpacing, 300, headerHeight, juce::Justification::centredLeft); // Row 5
    g.drawText("Gate", 30, startY + 5 * (cellHeight + rowSpacing) - headerHeight - headerToKnobSpacing, 300, headerHeight, juce::Justification::centredLeft); // Row 6
}

void DrumRAudioProcessorEditor::resized()
{
    const float phi = 1.61803398875f;
    const int knobWidth = 80;
    const int knobHeight = static_cast<int>(knobWidth / phi); // ≈ 49
    const int labelHeight = static_cast<int>(knobHeight / phi); // ≈ 30
    const int labelSpacing = 10;
    const int cellWidth = knobWidth;
    const int cellHeight = knobHeight + labelHeight + labelSpacing + 28; // ≈ 120, accounts for text box (120x28)
    const int titleFontSize = 36;
    const int rowSpacing = titleFontSize; // 36px, matches title font size
    const int startX = 30;
    const int startY = 20 + titleFontSize + rowSpacing; // 20 + 36 + 36 = 92

    juce::Grid grid;
    grid.templateRows = {
        juce::Grid::TrackInfo(juce::Grid::Px(cellHeight)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellHeight)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellHeight)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellHeight)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellHeight)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellHeight))
    };
    grid.templateColumns = {
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth)),
        juce::Grid::TrackInfo(juce::Grid::Px(cellWidth))
    };
    grid.rowGap = juce::Grid::Px(rowSpacing); // 36px
    grid.columnGap = juce::Grid::Px(static_cast<int>(knobWidth / phi)); // ≈ 49

    // Row 1: Input/Output
    grid.items.add(juce::GridItem(inputTrimContainer.get()).withArea(1, 1));
    grid.items.add(juce::GridItem(modeContainer.get()).withArea(1, 2));
    grid.items.add(juce::GridItem(outputTrimContainer.get()).withArea(1, 3));
    grid.items.add(juce::GridItem(dryWetContainer.get()).withArea(1, 4));
    grid.items.add(juce::GridItem(autoGainContainer.get()).withArea(1, 5));

    // Row 2: Filters + Transient
    grid.items.add(juce::GridItem(hpfHzContainer.get()).withArea(2, 1));
    grid.items.add(juce::GridItem(lpfHzContainer.get()).withArea(2, 2));
    grid.items.add(juce::GridItem(attAmtContainer.get()).withArea(2, 3));
    grid.items.add(juce::GridItem(susAmtContainer.get()).withArea(2, 4));

    // Row 3: Compressor
    grid.items.add(juce::GridItem(compThresholdContainer.get()).withArea(3, 1));
    grid.items.add(juce::GridItem(compRatioContainer.get()).withArea(3, 2));
    grid.items.add(juce::GridItem(compAttackContainer.get()).withArea(3, 3));
    grid.items.add(juce::GridItem(compReleaseContainer.get()).withArea(3, 4));
    grid.items.add(juce::GridItem(compKneeContainer.get()).withArea(3, 5));
    grid.items.add(juce::GridItem(scHpfHzContainer.get()).withArea(3, 6));
    grid.items.add(juce::GridItem(compMixContainer.get()).withArea(3, 7));

    // Row 4: Saturation
    grid.items.add(juce::GridItem(satTypeContainer.get()).withArea(4, 1));
    grid.items.add(juce::GridItem(satDriveContainer.get()).withArea(4, 2));
    grid.items.add(juce::GridItem(satBiasContainer.get()).withArea(4, 3));
    grid.items.add(juce::GridItem(satAsymContainer.get()).withArea(4, 4));
    grid.items.add(juce::GridItem(osFactorContainer.get()).withArea(4, 5));
    grid.items.add(juce::GridItem(satMixContainer.get()).withArea(4, 6));

    // Row 5: EQ + Clipper
    grid.items.add(juce::GridItem(tiltContainer.get()).withArea(5, 1));
    grid.items.add(juce::GridItem(bodyHzContainer.get()).withArea(5, 2));
    grid.items.add(juce::GridItem(bodyGainContainer.get()).withArea(5, 3));
    grid.items.add(juce::GridItem(clipCeilContainer.get()).withArea(5, 4));
    grid.items.add(juce::GridItem(limLookContainer.get()).withArea(5, 5));

    // Row 6: Gate
    grid.items.add(juce::GridItem(gateThreshContainer.get()).withArea(6, 1));
    grid.items.add(juce::GridItem(gateRatioContainer.get()).withArea(6, 2));
    grid.items.add(juce::GridItem(gateAttContainer.get()).withArea(6, 3));
    grid.items.add(juce::GridItem(gateRelContainer.get()).withArea(6, 4));
    grid.items.add(juce::GridItem(gateHystContainer.get()).withArea(6, 5));

    const int totalWidth = 7 * (cellWidth + static_cast<int>(knobWidth / phi)) - static_cast<int>(knobWidth / phi); // ≈ 872
    const int totalHeight = 6 * (cellHeight + rowSpacing) - rowSpacing; // ≈ 930

    grid.performLayout(juce::Rectangle<int>(startX, startY, totalWidth, totalHeight));
}

void DrumRAudioProcessorEditor::MetallicLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float startAngle, float endAngle, juce::Slider&)
{
    g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

    const float phi = 1.61803398875f;
    const float radius = juce::jmin(width, height) * 0.88f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = startAngle + sliderPos * (endAngle - startAngle);

    g.reduceClipRegion(juce::Rectangle<int>(static_cast<int>(rx), static_cast<int>(ry), static_cast<int>(rw), static_cast<int>(rw)));

    juce::ColourGradient grad(juce::Colour(0xFFCCCCCC), centreX, centreY,
        juce::Colour(0xFF888888), centreX + radius, centreY + radius / phi, true);
    g.setGradientFill(grad);
    g.fillEllipse(rx, ry, rw, rw);

    g.setColour(juce::Colour(0xFF333333));
    g.drawEllipse(rx, ry, rw, rw, 1.6f); // 2x original 0.8f

    const float indicatorLength = radius * (1.0f / phi);
    juce::Path indicator;
    indicator.addLineSegment(juce::Line<float>(0, -radius, 0, -radius + indicatorLength), 1.6f); // 2x original 0.8f
    indicator.applyTransform(juce::AffineTransform::rotation(angle, 0, 0));
    indicator.applyTransform(juce::AffineTransform::translation(centreX, centreY));
    g.setColour(juce::Colour(0xFF00A8CC));
    g.fillPath(indicator);
}