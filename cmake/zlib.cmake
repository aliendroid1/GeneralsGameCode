set(ZLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/zlib-1.1.4-src/ZLib)


add_library(libzlib STATIC)

target_sources(libzlib PRIVATE
    "${ZLIB_DIR}/adler32.c"
    "${ZLIB_DIR}/compress.c"
    "${ZLIB_DIR}/crc32.c"
    "${ZLIB_DIR}/gzio.c"
    "${ZLIB_DIR}/uncompr.c"
    "${ZLIB_DIR}/deflate.c"
    "${ZLIB_DIR}/trees.c"
    "${ZLIB_DIR}/zutil.c"
    "${ZLIB_DIR}/inflate.c"
    "${ZLIB_DIR}/infblock.c"
    "${ZLIB_DIR}/inftrees.c"
    "${ZLIB_DIR}/infcodes.c"
    "${ZLIB_DIR}/infutil.c"
    "${ZLIB_DIR}/inffast.c"
)

target_include_directories(libzlib PUBLIC ${ZLIB_DIR})

target_compile_definitions(libzlib PUBLIC Z_PREFIX)
