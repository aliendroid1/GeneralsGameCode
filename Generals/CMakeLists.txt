cmake_minimum_required(VERSION 3.25)

project(g_generals LANGUAGES C CXX)



# Set where the build results will end up
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Add main build targets
add_subdirectory(Code)

# If we are building on windows for windows, try and get the game install path from the registry.
if("${CMAKE_HOST_SYSTEM}" MATCHES "Windows" AND "${CMAKE_SYSTEM}" MATCHES "Windows")
    # Check the Steam registry path
    if(NOT RTS_INSTALL_PREFIX_GENERALS OR "${RTS_INSTALL_PREFIX_GENERALS}" STREQUAL "/registry")
        get_filename_component(RTS_INSTALL_PREFIX_GENERALS  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Electronic Arts\\EA Games\\Generals;installPath]" ABSOLUTE CACHE)
    endif()
endif()

if(RTS_INSTALL_PREFIX_GENERALS AND NOT "${RTS_INSTALL_PREFIX_GENERALS}" STREQUAL "/registry")
    install(TARGETS g_generals RUNTIME DESTINATION "${RTS_INSTALL_PREFIX_GENERALS}")
    install(FILES $<TARGET_PDB_FILE:g_generals> DESTINATION "${RTS_INSTALL_PREFIX_GENERALS}" OPTIONAL)
endif()


find_program(GRAPHVIZ dot
    HINTS ENV PATH
    DOC "Path to GraphViz 'dot' executable"
)

if (GRAPHVIZ)
  # ensure output directories exist
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/graphviz/dotFiles")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/graphviz/images")

  # custom target to generate .dot, .svg, and .png
  add_custom_target(graphviz
    COMMENT "Generating dependency graph with CMake+GraphViz"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    # 1) Dump the CMake target graph
    COMMAND ${CMAKE_COMMAND}
      --graphviz=graphviz/dotFiles/depTree.dot
      .
    # 2) Render to SVG
    COMMAND ${GRAPHVIZ}
      -Tsvg graphviz/dotFiles/depTree.dot
      -o graphviz/images/depTree.svg
    # 3) Render to PNG
    COMMAND ${GRAPHVIZ}
      -Tpng graphviz/dotFiles/depTree.dot
      -o graphviz/images/depTree.png
  )
endif()