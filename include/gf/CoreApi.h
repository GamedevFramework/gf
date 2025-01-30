// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_CORE_API_H
#define GF_CORE_API_H

#include "Portability.h"

#ifndef GF_STATIC
#  ifdef GF_BUILD
#    define GF_CORE_API GF_EXPORT
#  else
#    define GF_CORE_API GF_IMPORT
#  endif
#else
#  define GF_CORE_API
#endif

#endif // GF_CORE_API_H
