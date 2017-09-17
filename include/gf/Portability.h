/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef GF_PORTABILITY_H
#define GF_PORTABILITY_H

#ifdef _WIN32
  #define GF_API_EXPORT __declspec(dllexport)
  #define GF_API_IMPORT __declspec(dllimport)
#else
  #define GF_API_EXPORT
  #define GF_API_IMPORT
#endif

#if GF_EXPORT_SYMBOLS
  #define GF_API GF_API_EXPORT
#else
  #define GF_API GF_API_IMPORT
#endif


#ifdef __GNUC__
  #define GF_FORMAT(X, Y) __attribute__ ((format (printf, X, Y)))
#else
  #define GF_FORMAT(X, Y)
#endif

#endif // GF_PORTABILITY_H
