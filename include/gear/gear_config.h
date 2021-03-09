/*! \file gear_config.h
 *  \brief This header defines the general configuration shared between the Gear compiler and runtime.
 *
 *  This header defines the common configuration used by Gear.
 *  All of Gear's features are enabled by default, but this header can be edited to disable or
 *  reconfigure a feature.
 *  Alternatively, features can be disabled by passing the appropriate preprocessor directives
 *  during the build process.
 *
 *  \note
 *  If this header is modified, then the Gear compiler and runtime must both be rebuilt to reflect the changes.
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
#elif defined(GEAR_INT_LONG)
    typedef long gear_int;
    #define GEAR_FORMAT_INT "%ld"
    #define GEAR_STRING_TO_INT(s,p,b) strtol((s), (p), (b))
#elif defined(GEAR_INT_LONGLONG)
    #if defined(LLONG_MAX)
        typedef long long gear_int;
        #define GEAR_FORMAT_INT "%lld"
        #define GEAR_STRING_TO_INT(s,p,b) strtoll((s), (p), (b))
    #elif defined(_MSC_VER) /* The MSVC compiler defines its own 64-bit type and limits. */
        typedef __int64 gear_int;
        #define GEAR_FORMAT_INT "%I64d"
        #define GEAR_STRING_TO_INT(s,p,b) strtoll((s), (p), (b))
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

#endif
