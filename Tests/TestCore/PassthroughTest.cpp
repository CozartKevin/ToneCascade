#include <JuceHeader.h>
#include "PluginProcessor.h"

class PassthroughTest : public UnitTest {
public:
    PassthroughTest() : UnitTest("Passthrough Test") {}

    void runTest() override {
        ToneCascadeAudioProcessor processor;

        // Test 1: Verify silence-in → silence-out
        AudioBuffer<float> silentBuffer(2, 512); // 2 channels, 512 samples
        silentBuffer.clear();
        processor.processBlock(silentBuffer, MidiBuffer());
        for (int ch = 0; ch < silentBuffer.getNumChannels(); ++ch) {
            expectEquals(silentBuffer.getMagnitude(ch, 0, silentBuffer.getNumSamples()), 0.0f,
                "Silence input should produce silence output");
        }

        // Test 2: Verify signal preservation
        AudioBuffer<float> noiseBuffer(2, 512);
        for (int ch = 0; ch < noiseBuffer.getNumChannels(); ++ch) {
            FloatVectorOperations::fill(noiseBuffer.getWritePointer(ch), 0.5f, noiseBuffer.getNumSamples());
        }
        AudioBuffer<float> original = noiseBuffer; // Copy for comparison
        processor.processBlock(noiseBuffer, MidiBuffer());
        for (int ch = 0; ch < noiseBuffer.getNumChannels(); ++ch) {
            expect(FloatVectorOperations::compare(original.getReadPointer(ch),
                noiseBuffer.getReadPointer(ch),
                original.getNumSamples()) == 0,
                "Signal should pass through unchanged");
        }
    }
};

static PassthroughTest passthroughTest;