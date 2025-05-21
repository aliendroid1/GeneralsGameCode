# c stands for core, i stands for Interface
add_library(corei_libraries_include INTERFACE)
add_library(corei_always INTERFACE)

target_include_directories(corei_libraries_include INTERFACE "Libraries/Include")
target_link_libraries(corei_always INTERFACE corei_libraries_include)

# Set where the build results will end up
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Contains internal libraries
add_subdirectory(Libraries)




