





set(GAMEENGINE_SRC
)




add_library(corei_gameengine_private INTERFACE)
add_library(corei_gameengine_public INTERFACE)

target_sources(corei_gameengine_private INTERFACE ${GAMEENGINE_SRC})






target_link_libraries(corei_gameengine_public INTERFACE
    core_compression
    core_config
    core_browserdispatch
    d3d8lib
    gamespy::gamespy
)
