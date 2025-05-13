set(LZHL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/lzhl-src/CompLibHeader)

 
add_library(liblzhl STATIC)

target_sources(liblzhl PRIVATE
    "${LZHL_DIR}/_huff.h"
    "${LZHL_DIR}/_lz.h"
    "${LZHL_DIR}/_lzhl.h"
    "${LZHL_DIR}/Huff.cpp"
    "${LZHL_DIR}/Lz.cpp"
    #"${LZHL_DIR}/Lzhl_tcp.cpp"
    #"${LZHL_DIR}/Lzhl_tcp.h"
    "${LZHL_DIR}/Lzhl.cpp"
    "${LZHL_DIR}/lzhl.h"
)

target_include_directories(liblzhl PUBLIC ${LZHL_DIR} ${LZHL_DIR}/..)
