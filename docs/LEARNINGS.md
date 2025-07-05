# JUCE system install using CMAKE URL
https://ccrma.stanford.edu/~jos/juce_modules/md__Users_jos_w_JUCEModulesDoc_docs_CMake_API.html 

# CmakeList.txt
    ## ERROR: 
        CMake Error at D:/development/frameworks/JUCE/install/lib/cmake/JUCE-8.0.8/JUCEModuleSupport.cmake:48 (message):
        A C compiler is required to build JUCE.  Add 'C' to your project's
        LANGUAGES.
        Call Stack (most recent call first):
        D:/development/frameworks/JUCE/install/lib/cmake/JUCE-8.0.8/JUCEConfig.cmake:61 (include)
        CMakeLists.txt:5 (find_package)
    ## FIX:
    - Required to add C to the project(ToneCascade LANGUAGES CXX) line even though cmake version was 3.30.2 as it seems JUCE required C as CXX      didn't imply that. 