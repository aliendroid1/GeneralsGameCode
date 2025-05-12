# Do we want to build extra SDK stuff or just the game binary?
option(RTS_BUILD_CORE_TOOLS "Build core tools" OFF)
option(RTS_BUILD_CORE_EXTRAS "Build core extra tools/tests" OFF)
option(RTS_BUILD_ZEROHOUR "Build Zero Hour code." OFF)
option(RTS_BUILD_GENERALS "Build Generals code." ON)
option(RTS_BUILD_OPTION_INTERNAL "Build code with the \"Internal\" configuration." OFF)
option(RTS_BUILD_OPTION_PROFILE "Build code with the \"Profile\" configuration." OFF)
option(RTS_BUILD_OPTION_DEBUG "Build code with the \"Debug\" configuration." OFF)
option(RTS_BUILD_OPTION_ASAN "Build code with Address Sanitizer." OFF)
option(RTS_BUILD_OPTION_FFMPEG "Enable FFmpeg support" OFF)

if(NOT RTS_BUILD_ZEROHOUR AND NOT RTS_BUILD_GENERALS)
    set(RTS_BUILD_ZEROHOUR TRUE)
    message("You must select one project to build, building Zero Hour by default.")
endif()

add_feature_info(CoreTools RTS_BUILD_CORE_TOOLS "Build Core Mod Tools")
add_feature_info(CoreExtras RTS_BUILD_CORE_EXTRAS "Build Core Extra Tools/Tests")
add_feature_info(ZeroHourStuff RTS_BUILD_ZEROHOUR "Build Zero Hour code")
add_feature_info(GeneralsStuff RTS_BUILD_GENERALS "Build Generals code")
add_feature_info(InternalBuild RTS_BUILD_OPTION_INTERNAL "Building as an \"Internal\" build")
add_feature_info(ProfileBuild RTS_BUILD_OPTION_PROFILE "Building as a \"Profile\" build")
add_feature_info(DebugBuild RTS_BUILD_OPTION_DEBUG "Building as a \"Debug\" build")
add_feature_info(AddressSanitizer RTS_BUILD_OPTION_ASAN "Building with address sanitizer")
add_feature_info(FFmpegSupport RTS_BUILD_OPTION_FFMPEG "Building with FFmpeg support")


if(RTS_BUILD_GENERALS)
    option(RTS_BUILD_GENERALS_TOOLS "Build tools for Generals" OFF)
    option(RTS_BUILD_GENERALS_EXTRAS "Build extra tools/tests for Generals" OFF)
    option(RTS_BUILD_GENERALS_DOCS "Build documentation for Generals" OFF)

    add_feature_info(GeneralsTools RTS_BUILD_GENERALS_TOOLS "Build Generals Mod Tools")
    add_feature_info(GeneralsExtras RTS_BUILD_GENERALS_EXTRAS "Build Generals Extra Tools/Tests")
    add_feature_info(GeneralsDocs RTS_BUILD_GENERALS_DOCS "Build Generals Documentation")
endif()


target_compile_features(core_config INTERFACE cxx_std_20)

target_compile_options(core_config INTERFACE ${RTS_FLAGS})

# This disables a lot of warnings steering developers to use windows only functions/function names.
if(MSVC)
    target_compile_definitions(core_config INTERFACE _CRT_NONSTDC_NO_WARNINGS _CRT_SECURE_NO_WARNINGS $<$<CONFIG:DEBUG>:_DEBUG_CRT>)
endif()



if(RTS_BUILD_OPTION_DEBUG)
    target_compile_definitions(core_config INTERFACE RTS_DEBUG WWDEBUG DEBUG)
else()
    target_compile_definitions(core_config INTERFACE RTS_RELEASE)

    if(RTS_BUILD_OPTION_INTERNAL)
        target_compile_definitions(core_config INTERFACE RTS_INTERNAL)
    endif()
    if(RTS_BUILD_OPTION_PROFILE)
        target_compile_definitions(core_config INTERFACE RTS_PROFILE)
    endif()
endif()
