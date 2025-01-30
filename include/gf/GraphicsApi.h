// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_GRAPHICS_API_H
#define GF_GRAPHICS_API_H

#include "Portability.h"

#ifndef GF_STATIC
#  ifdef GF_BUILD
#    define GF_GRAPHICS_API GF_EXPORT
#  else
#    define GF_GRAPHICS_API GF_IMPORT
#  endif
#else
#  define GF_GRAPHICS_API
#endif

#endif // GF_GRAPHICS_API_H
