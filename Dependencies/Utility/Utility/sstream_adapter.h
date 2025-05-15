
#pragma once

#include <sstream>

using strstream = std::stringstream;

#define STRSTREAM_CSTR(_stringstream) _stringstream.str().c_str()
