/*! \file gear_config.h
 *  \brief This header defines the general configuration shared between the Gear compiler and runtime.
 *
 *  All features are enabled by default, but this header can be edited to disable or reconfigure a feature.
 *  Alternatively, features can be disabled by passing the appropriate preprocessor directives during compilation.
 *
 *  \note
 *  If this header is modified, then the Gear compiler, runtime, and tool suite must be completely rebuilt.
 */

#ifndef GEAR_CONFIG_API_H
#define GEAR_CONFIG_API_H

/* The "stdint.h" header (available in C99 and C++11) is needed to build Gear.
   If it's not available on your platform, then it must be created manually or downloaded. */
#include <stdint.h>

/*! \brief This preprocessor directive must be defined when building Gear as a dynamic link library (.DLL) on Windows.
 *
 *  This preprocessor directive is defined by default when building Gear using its build scripts. 
 *  If Gear is integrated with a foreign build system, then it must define this directive when
 *  building Gear as a dynamic library.
 */
#if defined(_WIN32) && defined(GEAR_SHARED_OBJECT)
#define GEAR_API __declspec(dllexport)
#else
#define GEAR_API
#endif

/*! \brief The compiled version of Gear as a string.
 *
 *  Do not assume this string is of the form "X.Y.Z" (where X, Y, Z are the major, minor, and patch versions).
 *  This is because it may contain letters, dashes, or other symbols indicating its alpha, beta, or preview status.
 *
 *  \note
 *  Gear adheres to semantic versioning.
 */
#define GEAR_VERSION_STRING "0.7.1 (pre-alpha)"

/*! \brief The major version of Gear.
 *
 *  The major version of Gear as an integer.
 */
#define GEAR_VERSION_MAJOR 0

/*! \brief The minor version of Gear.
 *
 *  The minor version of Gear as an integer.
 */
#define GEAR_VERSION_MINOR 7

/*! \brief The patch version of Gear.
 *
 *  The patch version of Gear as an integer.
 */
#define GEAR_VERSION_PATCH 1

/*! \brief Defines the Unicode character storage type used by Gear.
 *
 *  A "character" in Gear refers to a single Unicode code point.
 *  The largest Unicode code point is numerically defined is 0x10FFFF
 *  which requires an integer with at least 21-bits.
 *
 *  \note
 *  The typedef can be modified to use any integer type (signed or unsigned), but it must be at least 21-bits.
 *
 *  By default, this type aliases `uint_least32_t` making it identical with C11 and C++20's `char32_t` type.
 *  This allows UTF-32 encoded string literals `U"..."` and character literals `U' '` to be
 *  assigned to and from this type.
 */
typedef uint_least32_t gear_char;
#define GEAR_FORMAT_CHAR "%04X"

/*! \brief Defines the integer storage type used by the Gear.
 *
 *  Define one of the following preprocessor definitions to choose which 
 *  integer storage type is used by Gear's \c Int type.
 *  By default, Gear will use the largest integer type available.
 *
 *  Preprocessor Definition | C Storage Type
 *  ----------------------- | --------------
 *  GEAR_INT_INT            | int
 *  GEAR_INT_LONG           | long
 *  GEAR_INT_LONGLONG       | long long
 *
 *  The header can be modified to use any integer type, but it's must be a \e signed integer type.
 */
#ifdef DOXYGEN
typedef storage_type gear_int;
#endif
#if !defined(GEAR_INT_INT) && !defined(GEAR_INT_LONG) && !defined(GEAR_INT_LONGLONG)
#define GEAR_INT_LONGLONG /* The default configuration. */
#endif
#if defined(GEAR_INT_INT)
    typedef int gear_int;
    #define GEAR_FORMAT_INT "%d"
    #define GEAR_STRING_TO_INT(s,p,b) strtol((s), (p), (b))
    #define GEAR_INT_SUFFIX(N) N
#elif defined(GEAR_INT_LONG)
    typedef long gear_int;
    #define GEAR_FORMAT_INT "%ld"
    #define GEAR_STRING_TO_INT(s,p,b) strtol((s), (p), (b))
    #define GEAR_INT_SUFFIX(N) N##l
#elif defined(GEAR_INT_LONGLONG)
    typedef long long gear_int;
    #define GEAR_FORMAT_INT "%lld"
    #define GEAR_STRING_TO_INT(s,p,b) strtoll((s), (p), (b))
    #define GEAR_INT_SUFFIX(N) N##ll
#else
    #error "Numeric integer type not defined"
#endif

/*! \brief Defines the floating point storage type used by the Gear.
 *
 *  Define one of the following preprocessor definitions to choose which 
 *  floating point storage type is used by Gear's \c Float type.
 *  By default, Gear will pick the largest floating point type available.
 *
 *  Preprocessor Definition | C Storage Type
 *  ----------------------- | --------------
 *  GEAR_FLOAT_FLOAT        | float
 *  GEAR_FLOAT_DOUBLE       | double
 *  GEAR_FLOAT_LONGDOUBLE   | long double
 *
 *  The header can be modified to use any floating point type, but it's must be a \e signed type.
 */
#ifdef DOXYGEN
typedef storage_type gear_float;
#endif
#if !defined(GEAR_FLOAT_FLOAT) && !defined(GEAR_FLOAT_DOUBLE) && !defined(GEAR_FLOAT_LONGDOUBLE)
#define GEAR_FLOAT_LONGDOUBLE /* The default configuration. */
#endif
#if defined(GEAR_FLOAT_FLOAT)
    typedef float gear_float;
    #define GEAR_FORMAT_REAL "%.7g"
    #define GEAR_FLOAT_SUFFIX(F) F##f
#elif defined(GEAR_FLOAT_DOUBLE)
    typedef double gear_float;
    #define GEAR_FORMAT_REAL "%.14g"
    #define GEAR_FLOAT_SUFFIX(F) F
#elif defined(GEAR_FLOAT_LONGDOUBLE)
    typedef long double gear_float;
    #define GEAR_FORMAT_REAL "%.19Lg"
    #define GEAR_FLOAT_SUFFIX(F) F##L
#else
    #error "Numeric float type not defined"
#endif

#endif
