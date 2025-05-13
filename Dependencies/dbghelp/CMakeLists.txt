add_library(dbghelplib INTERFACE)
target_link_directories(dbghelplib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dbghelplib INTERFACE dbghelp)
