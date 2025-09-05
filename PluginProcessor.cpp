#include "PluginProcessor.h"
#include "PluginEditor.h"

/* ---------- fast utilities ---------- */
static inline float dbToLin(float dB) noexcept { return std::pow(10.0f, dB / 20.0f); }
static inline float softClip(float x) noexcept { return juce::jlimit(-1.0f, 1.0f, x - (x * x * x) * 0.3333333f); }

/* ---------- ctor ---------- */
DrumDesignerAudioProcessor::DrumDesignerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "PARAMS", createParameterLayout())
{
    // Initialize parameter pointers
    pInputTrim = apvts.getRawParameterValue("inputTrim");
    pMode = apvts.getRawParameterValue("mode");
    pHPFHz = apvts.getRawParameterValue("hpfHz");
    pLPFHz = apvts.getRawParameterValue("lpfHz");
    pAttAmt = apvts.getRawParameterValue("attAmt");
    pSusAmt = apvts.getRawParameterValue("susAmt");
    pCompThreshold = apvts.getRawParameterValue("compThreshold");
    pCompRatio = apvts.getRawParameterValue("compRatio");
    pCompAttack = apvts.getRawParameterValue("compAttack");
    pCompRelease = apvts.getRawParameterValue("compRelease");
    pCompKnee = apvts.getRawParameterValue("compKnee");
    pScHpfHz = apvts.getRawParameterValue("scHpfHz");
    pCompMix = apvts.getRawParameterValue("compMix");
    pSatType = apvts.getRawParameterValue("satType");
    pSatDrive = apvts.getRawParameterValue("satDrive");
    pSatBias = apvts.getRawParameterValue("satBias");
    pSatAsym = apvts.getRawParameterValue("satAsym");
    pOsFactor = apvts.getRawParameterValue("osFactor");
    pSatMix = apvts.getRawParameterValue("satMix");
    pTilt = apvts.getRawParameterValue("tilt");
    pBodyHz = apvts.getRawParameterValue("bodyHz");
    pBodyGain = apvts.getRawParameterValue("bodyGain");
    pGateThresh = apvts.getRawParameterValue("gateThresh");
    pGateRatio = apvts.getRawParameterValue("gateRatio");
    pGateAtt = apvts.getRawParameterValue("gateAtt");
    pGateRel = apvts.getRawParameterValue("gateRel");
    pGateHyst = apvts.getRawParameterValue("gateHyst");
    pClipCeil = apvts.getRawParameterValue("clipCeil");
    pLimLook = apvts.getRawParameterValue("limLook");
    pDryWet = apvts.getRawParameterValue("dryWet");
    pAutoGain = apvts.getRawParameterValue("autoGain");
    pOutputTrim = apvts.getRawParameterValue("outputTrim");
}

/* ---------- dtor ---------- */
DrumDesignerAudioProcessor::~DrumDesignerAudioProcessor()
{
    releaseResources();
}

/* ---------- parameter layout ---------- */
juce::AudioProcessorValueTreeState::ParameterLayout DrumDesignerAudioProcessor::createParameterLayout()
{
    using P = juce::AudioProcessorValueTreeState;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;

    auto choice = [](juce::String param, juce::String label, juce::StringArray opts, int def)
        {
            return std::make_unique<juce::AudioParameterChoice>(param, label, opts, def);
        };

    auto f = [](auto id, auto name, float min, float max, float def, float skew = 1.0f)
        {
            return std::make_unique<juce::AudioParameterFloat>(id, name, juce::NormalisableRange<float>(min, max, 0.0001f, skew), def);
        };

    auto b = [](auto id, auto name, bool def)
        {
            return std::make_unique<juce::AudioParameterBool>(id, name, def);
        };

    p.push_back(f("inputTrim", "Input", -24.f, 24.f, 0.f));
    p.push_back(choice("mode", "Mode", { "Kick", "Snare", "Tom", "Hat", "Perc", "Bus" }, 1));
    p.push_back(f("hpfHz", "HPF Hz", 10.f, 400.f, 30.f, 0.25f));
    p.push_back(f("lpfHz", "LPF Hz", 2000.f, 22000.f, 20000.f, 0.3f));
    p.push_back(f("attAmt", "Attack", -12.f, 12.f, 2.f));
    p.push_back(f("susAmt", "Sustain", -12.f, 12.f, 0.f));
    p.push_back(f("compThreshold", "Comp Thresh", -60.f, 0.f, -12.f));
    p.push_back(f("compRatio", "Comp Ratio", 1.0f, 20.0f, 4.0f, 0.35f));
    p.push_back(f("compAttack", "Comp Att ms", 0.1f, 60.f, 10.f, 0.35f));
    p.push_back(f("compRelease", "Comp Rel ms", 10.f, 500.f, 120.f, 0.35f));
    p.push_back(f("compKnee", "Comp Knee", 0.f, 24.f, 6.f));
    p.push_back(f("scHpfHz", "SC HPF Hz", 20.f, 400.f, 90.f, 0.35f));
    p.push_back(f("compMix", "Comp Mix", 0.f, 1.f, 1.f));
    p.push_back(choice("satType", "Saturation", { "Tanh", "Diode", "Tape" }, 0));
    p.push_back(f("satDrive", "Drive", 0.f, 36.f, 6.f));
    p.push_back(f("satBias", "Bias", -12.f, 12.f, 0.f));
    p.push_back(f("satAsym", "Asymmetry", 0.f, 1.f, 0.25f));
    p.push_back(choice("osFactor", "Oversampling", { "1x", "2x", "4x" }, 1));
    p.push_back(f("satMix", "Sat Mix", 0.f, 1.f, 0.8f));
    p.push_back(f("tilt", "Tilt", -6.f, 6.f, 0.f));
    p.push_back(f("bodyHz", "Body Hz", 60.f, 400.f, 180.f, 0.35f));
    p.push_back(f("bodyGain", "Body Gain", -12.f, 12.f, 0.f));
    p.push_back(f("gateThresh", "Gate Thresh", -60.f, 0.f, -40.f));
    p.push_back(f("gateRatio", "Gate Ratio", 1.f, 8.f, 2.f));
    p.push_back(f("gateAtt", "Gate Att ms", 0.1f, 20.f, 1.0f, 0.35f));
    p.push_back(f("gateRel", "Gate Rel ms", 5.f, 400.f, 120.f, 0.35f));
    p.push_back(f("gateHyst", "Gate Hyst dB", 0.f, 12.f, 3.f));
    p.push_back(f("clipCeil", "Ceiling dBFS", -12.f, -0.1f, -0.5f));
    p.push_back(f("limLook", "Lookahead ms", 0.0f, 3.0f, 0.5f));
    p.push_back(f("dryWet", "Dry/Wet", 0.f, 1.f, 1.f));
    p.push_back(b("autoGain", "Auto Gain", true));
    p.push_back(f("outputTrim", "Output", -24.f, 24.f, 0.f));

    return { p.begin(), p.end() };
}

/* ---------- prepare & cache atomics ---------- */
void DrumDesignerAudioProcessor::prepareToPlay(double sr, int block)
{
    if (sr <= 0.0 || block <= 0)
        return;

    updateDSP(sr, block);
}

/* ---------- release resources ---------- */
void DrumDesignerAudioProcessor::releaseResources()
{
    hpf.reset();
    lpf.reset();
    comp.reset();
    scHPF.reset();
    tiltLow.reset();
    tiltHigh.reset();
    bodyPeak.reset();
    envFast.reset();
    envSlow.reset();
    gateEnv.reset();
    oversampler.reset();
    dryBuffer.clear();
    scBuffer.clear();
    osBuffer.clear();
}

#if ! JucePlugin_IsMidiEffect
bool DrumDesignerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto& main = layouts.getMainOutputChannelSet();
    if (main != juce::AudioChannelSet::mono() && main != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}
#endif

/* ---------- DSP reset / resize ---------- */
void DrumDesignerAudioProcessor::updateDSP(double sr, int block)
{
    hpf.reset();
    lpf.reset();
    hpf.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    lpf.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

    const int numChannels = juce::jmax(1, getTotalNumInputChannels());
    juce::dsp::ProcessSpec spec{ sr, (juce::uint32)block, (juce::uint32)numChannels };

    hpf.prepare(spec);
    lpf.prepare(spec);

    envFast.prepare(sr);
    envSlow.prepare(sr);
    envFast.setTimes(1.0f, 30.0f);
    envSlow.setTimes(20.0f, 200.0f);

    comp.reset();
    comp.prepare(spec);

    try {
        scHPF.reset();
        tiltLow.reset();
        tiltHigh.reset();
        bodyPeak.reset();

        scHPF.prepare(spec);
        tiltLow.prepare(spec);
        tiltHigh.prepare(spec);
        bodyPeak.prepare(spec);

        scHPF.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(sr, pScHpfHz ? pScHpfHz->load() : 90.0f);
        tiltLow.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sr, 650.0f, 0.707f, 1.0f);
        tiltHigh.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sr, 650.0f, 0.707f, 1.0f);
        bodyPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sr, pBodyHz ? pBodyHz->load() : 180.0f, 1.2f, dbToLin(pBodyGain ? pBodyGain->load() : 0.0f));
    }
    catch (...) {
        scHPF.coefficients = juce::dsp::IIR::Coefficients<float>::makeAllPass(sr, 1000.0f);
        tiltLow.coefficients = juce::dsp::IIR::Coefficients<float>::makeAllPass(sr, 1000.0f);
        tiltHigh.coefficients = juce::dsp::IIR::Coefficients<float>::makeAllPass(sr, 1000.0f);
        bodyPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makeAllPass(sr, 1000.0f);
    }

    gateEnv.prepare(sr);
    gateEnv.setTimes(pGateAtt ? pGateAtt->load() : 1.0f, pGateRel ? pGateRel->load() : 120.0f);

    const int requestedOSFactor = pOsFactor ? (int)pOsFactor->load() == 2 ? 4 : (int)pOsFactor->load() == 1 ? 2 : 1 : 1;

    if (requestedOSFactor != lastOSFactor || !oversampler)
    {
        try {
            lastOSFactor = requestedOSFactor;
            oversampler.reset();
            if (lastOSFactor > 1) {
                oversampler = std::make_unique<juce::dsp::Oversampling<float>>(
                    numChannels,
                    juce::jmax(0, (int)std::log2(lastOSFactor)),
                    juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
                    true, false);
                oversampler->initProcessing(block);
            }
        }
        catch (...) {
            oversampler.reset();
            lastOSFactor = 1;
        }
    }

    const int safeChannels = juce::jmax(1, juce::jmin(8, numChannels));
    const int safeBlock = juce::jmax(1, juce::jmin(8192, block));

    osBuffer.setSize(safeChannels, safeBlock * lastOSFactor, false, false, true);
    dryBuffer.setSize(safeChannels, safeBlock, false, false, true);
    scBuffer.setSize(safeChannels, safeBlock, false, false, true);

    updateModeDefaults();
}

/* ---------- mode defaults (soft) ---------- */
void DrumDesignerAudioProcessor::updateModeDefaults()
{
    if (!pMode) return;

    const int mode = (int)pMode->load();
    auto setIfNear = [&](const char* id, float v, float tol = 0.01f)
        {
            auto* pr = apvts.getParameter(id);
            if (pr && std::abs(pr->getValue() - pr->convertTo0to1(v)) < tol)
                pr->setValueNotifyingHost(pr->convertTo0to1(v));
        };

    switch (mode)
    {
    case 0: setIfNear("hpfHz", 25.f); setIfNear("scHpfHz", 60.f); setIfNear("bodyHz", 80.f); break;
    case 1: setIfNear("hpfHz", 70.f); setIfNear("scHpfHz", 120.f); setIfNear("bodyHz", 180.f); break;
    case 2: setIfNear("hpfHz", 45.f); setIfNear("scHpfHz", 90.f); setIfNear("bodyHz", 140.f); break;
    case 3: setIfNear("hpfHz", 140.f); setIfNear("scHpfHz", 180.f); setIfNear("bodyHz", 260.f); break;
    case 4: setIfNear("hpfHz", 80.f); setIfNear("scHpfHz", 140.f); setIfNear("bodyHz", 200.f); break;
    default: setIfNear("hpfHz", 30.f); setIfNear("scHpfHz", 120.f); setIfNear("bodyHz", 160.f); break;
    }
}

/* ---------- processBlock ---------- */
void DrumDesignerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    const int numCh = buffer.getNumChannels();
    const int numSmps = buffer.getNumSamples();

    if (numCh <= 0 || numSmps <= 0 || !pInputTrim || !pOutputTrim)
    {
        buffer.clear();
        return;
    }

    juce::ScopedNoDenormals noDenormals;

    if (dryBuffer.getNumChannels() != numCh || dryBuffer.getNumSamples() != numSmps)
        dryBuffer.setSize(numCh, numSmps, false, false, true);
    if (scBuffer.getNumChannels() != numCh || scBuffer.getNumSamples() != numSmps)
        scBuffer.setSize(numCh, numSmps, false, false, true);

    const float inTrim = dbToLin(juce::jlimit(-48.0f, 48.0f, pInputTrim->load()));
    const float hpfHz = juce::jlimit(10.0f, 400.0f, pHPFHz ? pHPFHz->load() : 30.0f);
    const float lpfHz = juce::jlimit(2000.0f, 22000.0f, pLPFHz ? pLPFHz->load() : 20000.0f);
    const float attAmt = juce::jlimit(-12.0f, 12.0f, pAttAmt ? pAttAmt->load() : 0.0f);
    const float susAmt = juce::jlimit(-12.0f, 12.0f, pSusAmt ? pSusAmt->load() : 0.0f);
    const float compAttack = juce::jlimit(0.1f, 60.0f, pCompAttack ? pCompAttack->load() : 10.0f);
    const float compRel = juce::jlimit(10.0f, 500.0f, pCompRelease ? pCompRelease->load() : 120.0f);
    const float compThresh = juce::jlimit(-60.0f, 0.0f, pCompThreshold ? pCompThreshold->load() : -12.0f);
    const float compRatio = juce::jlimit(1.0f, 20.0f, pCompRatio ? pCompRatio->load() : 4.0f);
    const float compMix = juce::jlimit(0.0f, 1.0f, pCompMix ? pCompMix->load() : 1.0f);
    const int satType = juce::jlimit(0, 2, pSatType ? (int)pSatType->load() : 0);
    const float satDrive = juce::jlimit(0.0f, 36.0f, pSatDrive ? pSatDrive->load() : 6.0f);
    const float satBias = juce::jlimit(-12.0f, 12.0f, pSatBias ? pSatBias->load() : 0.0f);
    const float satAsym = juce::jlimit(0.0f, 1.0f, pSatAsym ? pSatAsym->load() : 0.25f);
    const float satMix = juce::jlimit(0.0f, 1.0f, pSatMix ? pSatMix->load() : 0.8f);
    const float tiltDb = juce::jlimit(-6.0f, 6.0f, pTilt ? pTilt->load() : 0.0f);
    const float bodyHz = juce::jlimit(60.0f, 400.0f, pBodyHz ? pBodyHz->load() : 180.0f);
    const float bodyGainDb = juce::jlimit(-12.0f, 12.0f, pBodyGain ? pBodyGain->load() : 0.0f);
    const float gateThresh = juce::jlimit(-60.0f, 0.0f, pGateThresh ? pGateThresh->load() : -40.0f);
    const float gateRatio = juce::jlimit(1.0f, 8.0f, pGateRatio ? pGateRatio->load() : 2.0f);
    const float gateHyst = juce::jlimit(0.0f, 12.0f, pGateHyst ? pGateHyst->load() : 3.0f);
    const float clipCeilDb = juce::jlimit(-12.0f, -0.1f, pClipCeil ? pClipCeil->load() : -0.5f);
    const float limLookMs = juce::jlimit(0.0f, 3.0f, pLimLook ? pLimLook->load() : 0.5f);
    const float dryWet = juce::jlimit(0.0f, 1.0f, pDryWet ? pDryWet->load() : 1.0f);
    const bool ag = pAutoGain ? (pAutoGain->load() > 0.5f) : true;
    float outTrim = juce::jlimit(-48.0f, 48.0f, pOutputTrim ? pOutputTrim->load() : 0.0f);

    dryBuffer.makeCopyOf(buffer);

    if (inTrim != 1.0f)
    {
        for (int ch = 0; ch < numCh; ++ch)
            buffer.applyGain(ch, 0, numSmps, inTrim);
    }

    try {
        hpf.setCutoffFrequency(hpfHz);
        lpf.setCutoffFrequency(lpfHz);
        juce::dsp::AudioBlock<float> blk(buffer);
        hpf.process(juce::dsp::ProcessContextReplacing<float>(blk));
        lpf.process(juce::dsp::ProcessContextReplacing<float>(blk));
    }
    catch (...) {
        buffer.clear();
        return;
    }

    try {
        doTransient(buffer, attAmt, susAmt);
        doCompressor(buffer, compMix, compAttack, compRel, compThresh, compRatio);
        doSaturator(buffer, satType, satDrive, satBias, satAsym, satMix);
        doTiltBody(buffer, tiltDb, bodyHz, bodyGainDb);
        doGate(buffer, gateThresh, gateRatio, gateHyst);
        const int lookSmps = (int)(limLookMs * 0.001f * getSampleRate());
        doClipLimit(buffer, clipCeilDb, lookSmps);
    }
    catch (...) {
        buffer.makeCopyOf(dryBuffer);
    }

    if (dryWet < 1.0f)
    {
        for (int ch = 0; ch < numCh; ++ch)
        {
            float* proc = buffer.getWritePointer(ch);
            const float* dry = dryBuffer.getReadPointer(ch);
            for (int i = 0; i < numSmps; ++i)
                proc[i] = proc[i] * dryWet + dry[i] * (1.0f - dryWet);
        }
    }

    if (ag)
    {
        const float est = -(0.35f * attAmt + 0.25f * susAmt + 0.3f * juce::jmax(0.0f, satDrive - 6.0f) + 2.0f * compMix);
        outTrim += juce::jlimit(-12.0f, 12.0f, est);
    }

    if (outTrim != 0.0f)
        buffer.applyGain(dbToLin(outTrim));
}

/* ---------- DSP sub-routines ---------- */
void DrumDesignerAudioProcessor::doTransient(juce::AudioBuffer<float>& buffer, float attAmt, float susAmt)
{
    const int numCh = buffer.getNumChannels();
    const int N = buffer.getNumSamples();

    for (int ch = 0; ch < numCh; ++ch)
    {
        float* x = buffer.getWritePointer(ch);
        for (int i = 0; i < N; ++i)
        {
            const float absx = std::fabs(x[i]);
            const float f = envFast.processAbs(absx);
            const float s = envSlow.processAbs(absx);
            const float delta = juce::jlimit(-1.0f, 1.0f, (f - s) * 5.0f);
            const float sustain = s;
            const float attGain = dbToLin(delta * attAmt);
            const float susGain = dbToLin((sustain - 0.1f) * susAmt * 0.5f);
            x[i] *= (attGain * susGain);
        }
    }
}

void DrumDesignerAudioProcessor::doCompressor(juce::AudioBuffer<float>& buffer, float compMix,
    float compAttack, float compRelease, float compThreshold, float compRatio)
{
    try {
        comp.setAttack(compAttack);
        comp.setRelease(compRelease);
        comp.setThreshold(compThreshold);
        comp.setRatio(compRatio);

        const int C = buffer.getNumChannels();
        const int N = buffer.getNumSamples();

        juce::AudioBuffer<float> compDryBuffer;
        compDryBuffer.setSize(C, N, false, false, true);
        compDryBuffer.makeCopyOf(buffer);

        scBuffer.makeCopyOf(buffer);
        for (int ch = 0; ch < C; ++ch)
        {
            float* scPtr = scBuffer.getWritePointer(ch);
            for (int i = 0; i < N; ++i)
            {
                scPtr[i] = scHPF.processSample(scPtr[i]);
            }
        }

        juce::dsp::AudioBlock<float> blk(buffer);
        comp.process(juce::dsp::ProcessContextReplacing<float>(blk));

        if (compMix < 1.0f)
        {
            for (int ch = 0; ch < C; ++ch)
            {
                float* proc = buffer.getWritePointer(ch);
                const float* dry = compDryBuffer.getReadPointer(ch);
                for (int i = 0; i < N; ++i)
                    proc[i] = proc[i] * compMix + dry[i] * (1.0f - compMix);
            }
        }
    }
    catch (...) {
        // Pass through on failure
    }
}

void DrumDesignerAudioProcessor::doSaturator(juce::AudioBuffer<float>& buffer,
    int typ, float driveDb, float biasDb, float asym, float mix)
{
    if (mix <= 0.0f) return;

    try {
        if (oversampler && lastOSFactor > 1)
        {
            juce::dsp::AudioBlock<float> inBlk(buffer);
            juce::dsp::AudioBlock<float> upBlk = oversampler->processSamplesUp(inBlk);

            const float drive = dbToLin(driveDb);
            const float bias = dbToLin(biasDb) - 1.0f;
            const size_t nch = upBlk.getNumChannels();
            const size_t ns = upBlk.getNumSamples();

            for (size_t ch = 0; ch < nch; ++ch)
            {
                float* d = upBlk.getChannelPointer(ch);
                for (size_t i = 0; i < ns; ++i)
                {
                    float x = d[i] * drive + bias;
                    float y;
                    if (typ == 0)      y = std::tanh(x);
                    else if (typ == 1)
                    {
                        const float pos = x > 0.f ? std::tanh(x * (1.0f + asym * 3.0f)) : x * 0.2f;
                        const float neg = x < 0.f ? std::tanh(x * (1.0f - asym * 2.0f)) : x * 0.2f;
                        y = (x >= 0.f ? pos : neg);
                    }
                    else y = x / (1.0f + 0.6f * std::fabs(x));
                    d[i] = juce::jmap(mix, x, y);
                }
            }
            oversampler->processSamplesDown(inBlk);
        }
        else
        {
            const float drive = dbToLin(driveDb);
            const int C = buffer.getNumChannels();
            const int N = buffer.getNumSamples();

            for (int ch = 0; ch < C; ++ch)
            {
                float* x = buffer.getWritePointer(ch);
                for (int i = 0; i < N; ++i)
                {
                    float sample = x[i] * drive;
                    float saturated = std::tanh(sample);
                    x[i] = juce::jmap(mix, x[i], saturated);
                }
            }
        }
    }
    catch (...) {
        // Pass through on failure
    }
}

void DrumDesignerAudioProcessor::doTiltBody(juce::AudioBuffer<float>& buffer, float tiltDb, float bodyHz, float bodyGainDb)
{
    try {
        tiltLow.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 650.0f, 0.707f, dbToLin(tiltDb));
        tiltHigh.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), 650.0f, 0.707f, dbToLin(-tiltDb));
        bodyPeak.coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), bodyHz, 1.2f, dbToLin(bodyGainDb));

        juce::dsp::AudioBlock<float> blk(buffer);
        tiltLow.process(juce::dsp::ProcessContextReplacing<float>(blk));
        tiltHigh.process(juce::dsp::ProcessContextReplacing<float>(blk));
        bodyPeak.process(juce::dsp::ProcessContextReplacing<float>(blk));
    }
    catch (...) {
        // Pass through on failure
    }
}

void DrumDesignerAudioProcessor::doGate(juce::AudioBuffer<float>& buffer, float gateThreshDb, float gateRatio, float gateHystDb)
{
    const int N = buffer.getNumSamples();
    const int C = buffer.getNumChannels();
    const float thr = dbToLin(gateThreshDb);
    const float thrClose = dbToLin(juce::Decibels::gainToDecibels(thr) - gateHystDb);

    for (int i = 0; i < N; ++i)
    {
        float s = 0.0f;
        for (int ch = 0; ch < C; ++ch)
            s += std::fabs(buffer.getReadPointer(ch)[i]);
        s *= (1.0f / (float)C);

        float e = gateEnv.processAbs(s);
        float g = (e > thr) ? 1.0f : (e < thrClose ? std::pow(e / thrClose, gateRatio) : 1.0f);

        for (int ch = 0; ch < C; ++ch)
            buffer.getWritePointer(ch)[i] *= g;
    }
}

void DrumDesignerAudioProcessor::doClipLimit(juce::AudioBuffer<float>& buffer, float ceilDb, int lookSmps)
{
    const float ceilLin = dbToLin(ceilDb);
    const int C = buffer.getNumChannels();
    const int N = buffer.getNumSamples();

    for (int ch = 0; ch < C; ++ch)
    {
        float* x = buffer.getWritePointer(ch);
        for (int i = 0; i < N; ++i)
        {
            const int lookAheadIdx = juce::jmin(i + lookSmps, N - 1);
            const float la = std::fabs(x[lookAheadIdx]);
            const float g = la > ceilLin ? ceilLin / la : 1.0f;
            float y = x[i] * g;
            y = softClip(y / ceilLin) * ceilLin;
            x[i] = y;
        }
    }
}

/* ---------- state ---------- */
void DrumDesignerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);
}

void DrumDesignerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
        apvts.replaceState(tree);
}

/* ---------- editor ---------- */
juce::AudioProcessorEditor* DrumDesignerAudioProcessor::createEditor()
{
    return new DrumRAudioProcessorEditor(*this);
}

/* ---------- plugin instantiation ---------- */
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DrumDesignerAudioProcessor();
}