/*! \file gear.h
 *  \brief This header includes both the Gear runtime and compiler API as well as API's to bridge them.
 *
 *  Consumers of this header should link against `gear.lib` on Windows and `libgear.a` on *nix systems.
 * 
 *  \warning
 *  Do \b not link the individual runtime (`libgearrt.a`) and compiler (`libgearc.a`) libraries.
 *  Doing so will produce duplicate symbol errors at program link time.
 */

#ifndef GEAR_API_H
#define GEAR_API_H

#include "gear_compiler.h"
#include "gear_runtime.h"

/*! \brief Compiles a Gear program and loads it directly into a Gear runtime.
 *
 *  This function can be thought of as being analogous to #gearc_build, but instead of writing a compiled program
 *  file it instead loads the compiled program into the given Gear runtime.
 *  Just like #gearc_build, the code must be compiled beforehand with #gearc_compile prior to calling this function.
 *
 *  This function is useful for embedders who would prefer to compile code on their users behalf.
 *  This can simplify a users workflow since they no longer need to compile the code themselves.
 *  This also means Gear source code can be loaded directly by native applications instead of already
 *  compiled Gear programs.
 *
 *  \param[in] runtime The Gear runtime to load the module into.
 *  \param[in] compiler The Gear compiler that compiled the code.
 *  \param[in] target The kind of target this is (e.g. executable, library, etc...).
 *  \since 0.1.0
 */
GEAR_API void gearc_build_to_runtime(gear_runtime *runtime, gear_compiler *compiler, gearc_target_type target);

#endif
