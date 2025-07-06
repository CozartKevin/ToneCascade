# JUCE system install using CMAKE URL
https://ccrma.stanford.edu/~jos/juce_modules/md__Users_jos_w_JUCEModulesDoc_docs_CMake_API.html 

# CmakeList.txt
    ## ERROR:  CMake Compiler C Error
        CMake Error at D:/development/frameworks/JUCE/install/lib/cmake/JUCE-8.0.8/JUCEModuleSupport.cmake:48 (message):
        A C compiler is required to build JUCE.  Add 'C' to your project's
        LANGUAGES.
        Call Stack (most recent call first):
        D:/development/frameworks/JUCE/install/lib/cmake/JUCE-8.0.8/JUCEConfig.cmake:61 (include)
        CMakeLists.txt:5 (find_package)

    ## FIX: Added C to Projects(ToneCascade LANGUAGES C CXX)
    - Required to add C to the project(ToneCascade LANGUAGES CXX) line even though cmake version was 3.30.2 as it seems JUCE required C as CXX      didn't imply that. 

    ## ERROR: No JuceHeader.h
        Build error warning about no JuceHeader.h
        D:\development\GitHub\Tone-Cascade\Source\PluginProcessor.h(11,10): error C1083: Cannot open include file: 'JuceHeader.
        h': No such file or directory [D:\development\GitHub\Tone-Cascade\build\ToneCascade.vcxproj]
        D:\development\GitHub\Tone-Cascade\Source\PluginProcessor.h(11,10): error C1083: Cannot open include file: 'JuceHeader.
        h': No such file or directory [D:\development\GitHub\Tone-Cascade\build\ToneCascade.vcxproj]

    ## FIX: Left over from ProJuicer project.  Implemented JuceInclude.h instead
    - This was due to me startin the project with Projuicer and not wiping the files that were generated when moving to CMake build.
      I implemented a JuceIncludes.h file with the needed includes and replaced all calls to JuceHeader.h with JuceInclude.h

    ## ERROR: FontOption not member of JUCE
        D:\development\GitHub\Tone-Cascade\Source\PluginEditor.cpp(32,22): error C2039: 'FontOptions': is not a member of 'juce

    ## FIX: Removed juce::FontOption() and used juce::Font()
    - modified Plugin editor g.setFont(Juce::FontOption (15.0f)) to be g.setFont(juce:Font(15.0f))
    - Left over issues from ProJucer setup

    ## ERROR: Issues with VST2 and not being able to exclude it
        D:\development\GitHub\Tone-Cascade\build\_deps\juce-src\modules\juce_audio_plugin_client\juce_audio_plugin_client_VST3.
        cpp(71,11): error C1083: Cannot open include file: 'pluginterfaces/vst2.x/vstfxstore.h': No such file or directory [D:\
        development\GitHub\Tone-Cascade\build\ToneCascade_VST3.vcxproj]

    ## FIX: " " Needed around VST3 Standaline in Formats area of juce_add_plugin()
    - upgrade project version to 8.0.8
    - Verified none of the .cpp/.h files had VST2 or VST listed in them
    - Added set(JUCE_ENABLE_VST2 OFF) to Cmakelist.txt
    - Added set(JUCE_BUILD_VST2 OFF) to Cmakelist.txt
    - Rolled back to 7.0.10
    - Modified FORMATS in CMakeList.txt from FORMATS VST3 Standalone to FORMATS "VST3 Standalone"


# Folder Structure
  - Source folder:   
        ⚡ Best practice confirmation:
            ✅ CORE/ → DSP, audio engine, data logic.
            ✅ UI/ → custom components, drawing, OpenGL visuals.
            ✅ Main.cpp, PluginProcessor.* → app/plugin entry points.
            ✅ PluginEditor.* → main editor window.
            ✅ JuceIncludes.h → one place, shared, keeps all includes tidy.