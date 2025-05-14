

# Print some information
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
if (DEFINED MSVC_VERSION)
    message(STATUS "MSVC_VERSION: ${MSVC_VERSION}")
endif()


set(IS_VS6_BUILD FALSE)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Ensures only ISO features are used


if (MSVC)
    # Multithreaded build.
    add_compile_options(/MP)
    # Enforce strict __cplusplus version
    add_compile_options(/Zc:__cplusplus)
endif()











add_library(core_config INTERFACE)



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

set(RTS_BUILD_GENERALS TRUE)

option(RTS_BUILD_GENERALS_TOOLS "Build tools for Generals" OFF)
option(RTS_BUILD_GENERALS_EXTRAS "Build extra tools/tests for Generals" OFF)
option(RTS_BUILD_GENERALS_DOCS "Build documentation for Generals" OFF)


    

target_compile_features(core_config INTERFACE cxx_std_20)
target_compile_options(core_config INTERFACE ${RTS_FLAGS})

# This disables a lot of warnings steering developers to use windows only functions/function names.
if(MSVC)
    target_compile_definitions(core_config INTERFACE _CRT_NONSTDC_NO_WARNINGS _CRT_SECURE_NO_WARNINGS $<$<CONFIG:DEBUG>:_DEBUG_CRT>)
endif()


target_compile_definitions(core_config INTERFACE RTS_DEBUG WWDEBUG DEBUG)

















set(RTS_DEBUG_LOGGING "DEFAULT" CACHE STRING "Enables debug logging. When DEFAULT, this option is enabled with DEBUG or INTERNAL")
set_property(CACHE RTS_DEBUG_LOGGING PROPERTY STRINGS DEFAULT ON OFF)

set(RTS_DEBUG_CRASHING "DEFAULT" CACHE STRING "Enables debug assert dialogs. When DEFAULT, this option is enabled with DEBUG or INTERNAL")
set_property(CACHE RTS_DEBUG_CRASHING PROPERTY STRINGS DEFAULT ON OFF)

set(RTS_DEBUG_STACKTRACE "DEFAULT" CACHE STRING "Enables debug stacktracing. This inherintly also enables debug logging. When DEFAULT, this option is enabled with DEBUG or INTERNAL")
set_property(CACHE RTS_DEBUG_STACKTRACE PROPERTY STRINGS DEFAULT ON OFF)

set(RTS_DEBUG_PROFILE "DEFAULT" CACHE STRING "Enables debug profiling. When DEFAULT, this option is enabled with DEBUG or INTERNAL")
set_property(CACHE RTS_DEBUG_PROFILE PROPERTY STRINGS DEFAULT ON OFF)

option(RTS_DEBUG_INCLUDE_DEBUG_LOG_IN_CRC_LOG "Includes normal debug log in crc log" OFF)
option(RTS_DEBUG_MULTI_INSTANCE "Enables multi client instance support" OFF)


# Helper macro that handles DEFAULT ON OFF options
macro(define_debug_option OptionName OptionEnabledCompileDef OptionDisabledCompileDef FeatureInfoName FeatureInfoDescription)
    if(${OptionName} STREQUAL "DEFAULT")
        # Does nothing
    elseif(${OptionName} STREQUAL "ON")
        target_compile_definitions(core_config INTERFACE ${OptionEnabledCompileDef}=1)
        add_feature_info(${FeatureInfoName} TRUE ${FeatureInfoDescription})
    elseif(${OptionName} STREQUAL "OFF")
        target_compile_definitions(core_config INTERFACE ${OptionDisabledCompileDef}=1)
        add_feature_info(${FeatureInfoName} FALSE ${FeatureInfoDescription})
    else()
        message(FATAL_ERROR "Unhandled option")
    endif()
endmacro()


define_debug_option(RTS_DEBUG_LOGGING    DEBUG_LOGGING    DISABLE_DEBUG_LOGGING    DebugLogging    "Build with Debug Logging")
define_debug_option(RTS_DEBUG_CRASHING   DEBUG_CRASHING   DISABLE_DEBUG_CRASHING   DebugCrashing   "Build with Debug Crashing")
define_debug_option(RTS_DEBUG_STACKTRACE DEBUG_STACKTRACE DISABLE_DEBUG_STACKTRACE DebugStacktrace "Build with Debug Stacktracing")
define_debug_option(RTS_DEBUG_PROFILE    DEBUG_PROFILE    DISABLE_DEBUG_PROFILE    DebugProfile    "Build with Debug Profiling")

add_feature_info(DebugIncludeDebugLogInCrcLog RTS_DEBUG_INCLUDE_DEBUG_LOG_IN_CRC_LOG "Build with Debug Logging in CRC log")
add_feature_info(DebugMultiInstance RTS_DEBUG_MULTI_INSTANCE "Build with Multi Client Instance support")


if(RTS_DEBUG_INCLUDE_DEBUG_LOG_IN_CRC_LOG)
    target_compile_definitions(core_config INTERFACE INCLUDE_DEBUG_LOG_IN_CRC_LOG)
endif()

if(RTS_DEBUG_MULTI_INSTANCE)
    target_compile_definitions(core_config INTERFACE RTS_MULTI_INSTANCE)
endif()
















# Game Memory features
option(RTS_GAMEMEMORY_ENABLE "Enables the memory pool and dynamic memory allocator." OFF)

# Memory pool features
option(RTS_MEMORYPOOL_OVERRIDE_MALLOC "Enables the Dynamic Memory Allocator for malloc calls." OFF)
option(RTS_MEMORYPOOL_MPSB_DLINK "Adds a backlink to MemoryPoolSingleBlock. Makes it faster to free raw DMA blocks, but increases memory consumption." OFF)

# Memory pool debugs
option(RTS_MEMORYPOOL_DEBUG "Enables Memory Pool debug." OFF)
option(RTS_MEMORYPOOL_DEBUG_CUSTOM_NEW "Enables a custom new operator for the Memory Pool." OFF)
option(RTS_MEMORYPOOL_DEBUG_CHECKPOINTING "Records checkpoint information about the history of memory allocations." OFF)
option(RTS_MEMORYPOOL_DEBUG_BOUNDINGWALL "Enables bounding wall checks around memory chunks to find memory trampling." OFF)
option(RTS_MEMORYPOOL_DEBUG_STACKTRACE "Enables stack trace collection for allocations. Reduces runtime performance significantly." OFF)
option(RTS_MEMORYPOOL_DEBUG_INTENSE_VERIFY "Enables intensive verifications after nearly every memory operation. OFF by default, since it slows down things a lot, but is worth turning on for really obscure memory corruption issues." OFF)
option(RTS_MEMORYPOOL_DEBUG_CHECK_BLOCK_OWNERSHIP "Enables debug to verify that a block actually belongs to the pool it is called with. This is great for debugging, but can be realllly slow, so is OFF by default." OFF)
option(RTS_MEMORYPOOL_DEBUG_INTENSE_DMA_BOOKKEEPING "Prints statistics for memory usage of Memory Pools." OFF)

# Game Memory features
add_feature_info(GameMemoryEnable RTS_GAMEMEMORY_ENABLE "Build with the original game memory implementation")

# Memory pool features
add_feature_info(MemoryPoolOverrideMalloc RTS_MEMORYPOOL_OVERRIDE_MALLOC "Build with Memory Pool malloc")
add_feature_info(MemoryPoolMpsbDlink RTS_MEMORYPOOL_MPSB_DLINK "Build with Memory Pool backlink")

# Memory pool debugs
add_feature_info(MemoryPoolDebug RTS_MEMORYPOOL_DEBUG "Build with Memory Pool debug")
add_feature_info(MemoryPoolDebugCustomNew RTS_MEMORYPOOL_DEBUG_CUSTOM_NEW "Build with Memory Pool custom new")
add_feature_info(MemoryPoolDebugCheckpointing RTS_MEMORYPOOL_DEBUG_CHECKPOINTING "Build with Memory Pool checkpointing")
add_feature_info(MemoryPoolDebugBoundingwall RTS_MEMORYPOOL_DEBUG_BOUNDINGWALL "Build with Memory Pool Bounding Wall")
add_feature_info(MemoryPoolDebugStacktrace RTS_MEMORYPOOL_DEBUG_STACKTRACE "Build with Memory Pool Stacktrace")
add_feature_info(MemoryPoolDebugIntenseVerify RTS_MEMORYPOOL_DEBUG_INTENSE_VERIFY "Build with Memory Pool intense verify")
add_feature_info(MemoryPoolDebugCheckBlockOwnership RTS_MEMORYPOOL_DEBUG_CHECK_BLOCK_OWNERSHIP "Build with Memory Pool block ownership checks")
add_feature_info(MemoryPoolDebugIntenseDmaBookkeeping RTS_MEMORYPOOL_DEBUG_INTENSE_DMA_BOOKKEEPING "Build with Memory Pool intense DMA bookkeeping")


# Game Memory features
if(NOT RTS_GAMEMEMORY_ENABLE)
    target_compile_definitions(core_config INTERFACE DISABLE_GAMEMEMORY=1)
endif()

# Memory pool features
if(RTS_MEMORYPOOL_OVERRIDE_MALLOC)
    target_compile_definitions(core_config INTERFACE MEMORYPOOL_OVERRIDE_MALLOC=1)
endif()

if(NOT RTS_MEMORYPOOL_MPSB_DLINK)
    target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_MPSB_DLINK=1)
endif()

# Memory pool debugs
if(NOT RTS_MEMORYPOOL_DEBUG)
    target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_DEBUG=1)
else()
    if(NOT RTS_MEMORYPOOL_DEBUG_CUSTOM_NEW)
        target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_DEBUG_CUSTOM_NEW=1)
    endif()
    
    if(RTS_MEMORYPOOL_DEBUG_CHECKPOINTING)
        # Set to 0 to override the default setting in code
        target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_CHECKPOINTING=0)
    else()
        target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_CHECKPOINTING=1)
    endif()
    
    if(NOT RTS_MEMORYPOOL_DEBUG_BOUNDINGWALL)
        target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_BOUNDINGWALL=1)
    endif()
    
    if(NOT RTS_MEMORYPOOL_DEBUG_STACKTRACE)
        target_compile_definitions(core_config INTERFACE DISABLE_MEMORYPOOL_STACKTRACE=1)
    endif()
    
    if(RTS_MEMORYPOOL_DEBUG_INTENSE_VERIFY)
        target_compile_definitions(core_config INTERFACE MEMORYPOOL_INTENSE_VERIFY=1)
    endif()
    
    if(RTS_MEMORYPOOL_DEBUG_CHECK_BLOCK_OWNERSHIP)
        target_compile_definitions(core_config INTERFACE MEMORYPOOL_CHECK_BLOCK_OWNERSHIP=1)
    endif()
    
    if(RTS_MEMORYPOOL_DEBUG_INTENSE_DMA_BOOKKEEPING)
        target_compile_definitions(core_config INTERFACE INTENSE_DMA_BOOKKEEPING=1)
    endif()
endif()
