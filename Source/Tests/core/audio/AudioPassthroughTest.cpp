#include <JuceIncludes.h>
#include "../Source/Core/PluginProcessor.h"

namespace tc::test {

    class AudioPassthroughTest : public juce::UnitTest {
    public:
        AudioPassthroughTest()
            : juce::UnitTest("AudioPassthrough", "tc.audio") {}

        void runTest() override {
            beginTest("Initialization");
            testInitialization();

            beginTest("BypassBehavior");
            testBypassBehavior();

            beginTest("SampleAccuracy");
            testSampleAccuracy();
        }

    private:
        void testInitialization() {
            ToneCascadeAudioProcessor proc;
            const double testRate = 48000.0;

            proc.prepareToPlay(testRate, 512);
            expectWithinAbsoluteError(
                proc.getSampleRate(), // Make sure this matches your method
                testRate,
                0.001,
                "Sample rate not properly configured"
            );
        }

        void testBypassBehavior() {
            ToneCascadeAudioProcessor proc;
            proc.prepareToPlay(44100.0, 512);

            juce::AudioBuffer<float> buffer(2, 512);
            juce::MidiBuffer midiBuffer;
            buffer.clear();
            buffer.applyGain(0.5f);

            // Use the new setBypassed method
            proc.setBypassed(true);
            proc.processBlock(buffer, midiBuffer);

            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                const auto* channelData = buffer.getReadPointer(ch);
                for (int i = 0; i < buffer.getNumSamples(); ++i) {
                    expectWithinAbsoluteError(
                        channelData[i],
                        0.0f,
                        1e-6f,
                        "Bypass failed to mute output"
                    );
                }
            }
        }

        void testSampleAccuracy() {
            ToneCascadeAudioProcessor proc;
            proc.prepareToPlay(96000.0, 1024);
            proc.setBypassed(false);

            juce::AudioBuffer<float> buffer(2, 1024);
            juce::Random rng(12345);
            juce::MidiBuffer midiBuffer;

            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                auto* channelData = buffer.getWritePointer(ch);
                for (int i = 0; i < buffer.getNumSamples(); ++i) {
                    channelData[i] = rng.nextFloat() * 2.0f - 1.0f;
                }
            }

            const juce::AudioBuffer<float> original(buffer);
            proc.processBlock(buffer, midiBuffer);

            for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
                const auto* originalData = original.getReadPointer(ch);
                const auto* processedData = buffer.getReadPointer(ch);
                for (int i = 0; i < buffer.getNumSamples(); ++i) {
                    expectEquals(processedData[i], originalData[i],
                        juce::String::formatted(
                            "Sample corruption at ch.%d[%d]",
                            ch, i
                        )
                    );
                }
            }
        }
    };

    static AudioPassthroughTest passthroughTest;
} // namespace tc::test