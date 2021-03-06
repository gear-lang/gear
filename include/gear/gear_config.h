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

/*! \brief This preprocessor directive must be defined when building Gear as a dynamic link library (.DLL) on Windows.
 *
 *  This preprocessor directive is defined by default when building Gear using its build scripts. 
 *  If Gear is being integrated with a different build system, then it should define this directive.
 *  This directive should \b not be defined if Gear is being built as a static library.
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

/*! \brief A boolean type compatible with pre-C99 compilers.
 *
 *  Gear aliases C's `_Bool` type when building with a compiler supporting C99 or newer.
 *  It aliases an integer type when compiled with a pre-C99 compiler.
 *  User's of these older compilers should use the integer `0` for false and `1` for true.
 *  User's of C99 (or newer) can do the same or use the `true` and `false` macros defined in `stdbool.h`.
 */
#ifdef DOXYGEN
typedef _Bool gear_bool;
#endif
/* The Microsoft Visual C++ compiler does not define the __STDC_VERSION__ macro despite supporting
   most C99 feature since MSVC 2013. This means an additional _MSC_VER check is required below. */
#ifdef __cplusplus
typedef bool gear_bool;
#elif (__STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
typedef _Bool gear_bool;
#else
typedef unsigned char gear_bool;
#endif

/*! \brief A single Unicode code point.
 *
 *  A "character" in Gear refers to a single Unicode code point.
 *  This typedef defines an integer large enough to hold one code point.
 *
 *  Unicode defines code points to be at most 21-bits with the largest being 0x10FFFF.
 *  C's `long` type is used because it's guaranteed to be at least 32-bits.
 */
typedef long gear_char;

/*! \brief Defines the integer storage type used by the Gear runtime.
 * 
 *  Define one of the following preprocessor definitions to choose which 
 *  integer storage type is used by Gear's \c Int type.
 *  By default, Gear will pick the largest integer type available.
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
    #if defined(LLONG_MAX)
        typedef long long gear_int;
        #define GEAR_FORMAT_INT "%lld"
        #define GEAR_STRING_TO_INT(s,p,b) strtoll((s), (p), (b))
        #define GEAR_INT_SUFFIX(N) N##ll
    #elif defined(_MSC_VER) /* The MSVC compiler defines its own 64-bit type and limits. */
        typedef __int64 gear_int;
        #define GEAR_FORMAT_INT "%I64d"
        #define GEAR_STRING_TO_INT(s,p,b) strtoll((s), (p), (b))
        #define GEAR_INT_SUFFIX(N) N##ll
    #else
        #error "Compiler does not support 'long long'. Use option '-DGEAR_INT_LONG' (see 'gear_config.h' for details)"
    #endif
#endif

 /*! \brief Defines the floating point storage type used by the Gear runtime.
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

 /*! \brief Determines if the compiler is Unicode aware.
 * 
 *  The Gear compiler will build without Unicode support if this preprocessor directive is defined.
 *  By default, the Gear compiler will build with full Unicode support.
 *  This means it will recognize Unicode space characters and line endings as well as
 *  alphanumeric characters and Emoji in identifiers.
 *
 *  \note
 *  The Gear compiler \e always allows Unicode characters inside strings and comments
 *  regardless of whether this feature is enabled or disabled.
 *  This feature has no effect on the runtime.
 *  The C API can still lookup symbols with Unicode characters so long as the given C string is valid UTF-8.
 *
 *  Disabling this feature decreases the size of the compiler by around 200 KB!
 *  For this reason, if the compiler is being embedded on a system with limited resources, then
 *  this feature can be disabled to save space.
 */
#ifdef DOXYGEN
#define GEAR_DISABLE_UNICODE_COMPILER
#endif
//#define GEAR_DISABLE_UNICODE_COMPILER

/*! \brief Determines if the string library is Unicode aware.
 *
 *  The string library will build without Unicode support if this preprocessor directive is defined.
 *  By default, the Gear string library will build with full Unicode support.
 *  That means character query and transformation functions like isDigit() and toUpper() will understand Unicode
 *  characters. If this feature is disabled, then the string library will only understand ASCII.
 *
 *  \note
 *  Gear strings are \e always UTF-8 encoded and the string library \e always understands how to index
 *  Unicode characters regardless of whether this feature is enabled or disabled.
 *  This feature solely affects the behavior of character query and transformation functions.
 *
 *  Disabling this feature decreases the size of the runtime by around 200 KB!
 *  For this reason, if the runtime is being embedded on a system with limited resources and/or does not need 
 *  Unicode aware character transformation and query functions, then this feature can be disabled to reduce
 *  Gear's footprint.
 */
#ifdef DOXYGEN
#define GEAR_DISABLE_UNICODE_STRING_LIBRARY
#endif
//#define GEAR_DISABLE_UNICODE_STRING_LIBRARY

/*! \brief Determines if Unicode code point data needed by the Gear runtime is tightly packed in memory.
 *
 *  The Unicode code point data set needed by the Gear runtime can be tightly packed to reduce its size.
 *  Doing so reduces its footprint by ~25%, but at the expense of performance since unaligned memory access
 *  is slower on architectures that permit it (like x86 and amd64).
 *
 *  \note
 *  Strict alignment architectures, like SPARC, do not support unaligned memory access.
 *
 *  This feature is disabled by default because it may or may not be supported by the systems architecture.
 */
#ifdef DOXYGEN
#define GEAR_COMPRESS_UNICODE_TABLE
#endif
//#define GEAR_COMPRESS_UNICODE_TABLE

#endif
