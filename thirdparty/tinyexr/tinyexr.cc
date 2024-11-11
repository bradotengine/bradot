#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

// -- BRADOT start --
#include <zlib.h> // Should come before including tinyexr.
// -- BRADOT end --

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"
