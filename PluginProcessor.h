#pragma once
#include <JuceHeader.h>

// Simple envelope follower for transient detection
class EnvelopeFollower
{
public:
    void prepare(double sampleRate)
    {
        sr = sampleRate;
        state = 0.0f;
    }

    void setTimes(float attackMs, float releaseMs)
    {
        attack = std::exp(-1.0f / (attackMs * 0.001f * (float)sr));
        release = std::exp(-1.0f / (releaseMs * 0.001f * (float)sr));
    }

    float processAbs(float x)
    {
        const float coeff = (x > state) ? attack : release;
        state = x + coeff * (state - x);
        return state;
    }

    void reset() { state = 0.0f; }

private:
    double sr = 44100.0;
    float attack = 0.0f, release = 0.0f;
    float state = 0.0f;
};

// DrumDesigner Audio Processor - Professional drum shaping plugin
class DrumDesignerAudioProcessor : public juce::AudioProcessor
{
public:
    DrumDesignerAudioProcessor();
    ~DrumDesignerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#if ! JucePlugin_IsMidiEffect
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override
    {
#if JucePlugin_WantsMidiInput
        return true;
#else
        return false;
#endif
    }

    bool producesMidi() const override
    {
#if JucePlugin_ProducesMidiOutput
        return true;
#else
        return false;
#endif
    }

    bool isMidiEffect() const override
    {
#if JucePlugin_IsMidiEffect
        return true;
#else
        return false;
#endif
    }

    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void updateDSP(double sampleRate, int samplesPerBlock);
    void updateModeDefaults();

    void doTransient(juce::AudioBuffer<float>& buffer, float attAmt, float susAmt);
    void doCompressor(juce::AudioBuffer<float>& buffer, float compMix,
        float compAttack, float compRelease, float compThreshold, float compRatio);
    void doSaturator(juce::AudioBuffer<float>& buffer, int typ, float driveDb,
        float biasDb, float asym, float mix);
    void doTiltBody(juce::AudioBuffer<float>& buffer, float tiltDb, float bodyHz, float bodyGainDb);
    void doGate(juce::AudioBuffer<float>& buffer, float gateThreshDb, float gateRatio, float gateHystDb);
    void doClipLimit(juce::AudioBuffer<float>& buffer, float ceilDb, int lookSmps);

    // DSP components
    juce::dsp::StateVariableTPTFilter<float> hpf, lpf;
    juce::dsp::Compressor<float> comp;
    juce::dsp::IIR::Filter<float> scHPF, tiltLow, tiltHigh, bodyPeak;
    EnvelopeFollower envFast, envSlow, gateEnv;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    int lastOSFactor = 1;

    // Buffers
    juce::AudioBuffer<float> dryBuffer, scBuffer, osBuffer;

    // Parameter pointers
    std::atomic<float>* pInputTrim = nullptr;
    std::atomic<float>* pMode = nullptr;
    std::atomic<float>* pHPFHz = nullptr;
    std::atomic<float>* pLPFHz = nullptr;
    std::atomic<float>* pAttAmt = nullptr;
    std::atomic<float>* pSusAmt = nullptr;
    std::atomic<float>* pCompThreshold = nullptr;
    std::atomic<float>* pCompRatio = nullptr;
    std::atomic<float>* pCompAttack = nullptr;
    std::atomic<float>* pCompRelease = nullptr;
    std::atomic<float>* pCompKnee = nullptr;
    std::atomic<float>* pScHpfHz = nullptr;
    std::atomic<float>* pCompMix = nullptr;
    std::atomic<float>* pSatType = nullptr;
    std::atomic<float>* pSatDrive = nullptr;
    std::atomic<float>* pSatBias = nullptr;
    std::atomic<float>* pSatAsym = nullptr;
    std::atomic<float>* pOsFactor = nullptr;
    std::atomic<float>* pSatMix = nullptr;
    std::atomic<float>* pTilt = nullptr;
    std::atomic<float>* pBodyHz = nullptr;
    std::atomic<float>* pBodyGain = nullptr;
    std::atomic<float>* pGateThresh = nullptr;
    std::atomic<float>* pGateRatio = nullptr;
    std::atomic<float>* pGateAtt = nullptr;
    std::atomic<float>* pGateRel = nullptr;
    std::atomic<float>* pGateHyst = nullptr;
    std::atomic<float>* pClipCeil = nullptr;
    std::atomic<float>* pLimLook = nullptr;
    std::atomic<float>* pDryWet = nullptr;
    std::atomic<float>* pAutoGain = nullptr;
    std::atomic<float>* pOutputTrim = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumDesignerAudioProcessor)
};