/*! \file gear_compiler.h
 *  \brief This header defines the Gear Compiler API.
 *
 *  This header defines the API for managing a Gear compiler.
 *  Consumers of this header must link against `gearc.lib` on Windows and `libgearc.a` on *nix systems.
 */

#ifndef GEAR_COMPILER_API_H
#define GEAR_COMPILER_API_H

#include "gear_config.h"

// Type aliases for enumerations are defined seperatly from their definitions to prevent Doxygen
// from generating duplicate documention: once for the typedef and again for the enumeration.
typedef enum gear_warning_type gear_warning_type;
typedef enum gearc_unit_property gearc_unit_property;
typedef enum gearc_target_type gearc_target_type;

/*! \brief This enumeration defines the complete list of warnings the Gear compiler can emit.
 *
 *  The elements of this enumeration should be used in conjunction with the functions
 *  #gearc_warning_toggle and #gearc_warning_status to modify or retrieve the on/off
 *  state of each compiler warning.
 *
 *  \sa gearc_warning_toggle
 *  \sa gearc_warning_status
 */
enum gear_warning_type
{
    /*! \brief Represents all warnings.
     *
     *  This is not a warning, but a special value that represents all warnings.
     *  It can be used to enable or disable all warnings at once.
     *  This avoids having to manually toggle on or off each warning individually.
     */
    GEAR_WARN_ALL = -1,

    /*! \brief Warns the user about unused function parameters.
     *
     *  A warning is reported when parameters are not used.
     *  This is useful for alerting the programmer to legacy parameters.
     *  Example:
     *
     * \code{.gear}
     * // warning: unused parameter 'y'
     * func sum(x: Int, y: Int) -> Int {
     *     return x;
     * }
     * \endcode
     */
    GEAR_WARN_UNUSED_PARAMS,

    /*! \brief Warns the user about unused variables.
     *
     *  Reports a warning when a variable is declared but not used.
     *  This is useful for alerting the programmer to legacy parameters.
     *  Example:
     *
     * \code{.gear}
     * func sayHi() {
     *     var x: Int; // warning: unused variable 'x'
     *     print("Hi!");
     * }
     * \endcode
     */
    GEAR_WARN_UNUSED_VARIABLES,

    /*! \brief Warns the user about expressions with unused results.
     *
     *  A warning is reported when a side-effect free expression is used without using the result.
     *  Example:
     *
     * \code{.gear}
     * func noop() {
     *     x + 1; // warning: unused expression
     * }
     * \endcode
     */
    GEAR_WARN_UNUSED_EXPRESSIONS,

    /*! \brief Warns the user about duplicate import statements.
     *
     *  A warning is reported when redundant import statements are used.
     *  Example:
     *
     * \code{.gear}
     * import foobar;
     * import foobar; // warning: duplicate import
     * \endcode
     */
    GEAR_WARN_DUPLICATE_IMPORTS,

    /*! \brief Warns the user about constant expressions in conditionals.
     *
     *  A warning is reported when a conditional uses a constant expression.
     *  Constant expressions will always evaluate to either true or false.
     *  Example:
     *
     * \code{.gear}
     * if 1 { // warning: constant conditionals
     * }
     * \endcode
     */
    GEAR_WARN_CONSTANT_CONDITION,

    /*! \brief Warns the user about out of range numeric literals.
     *
     *  A warning is reported when a fixed with numeric literal exceeds its maximum value.
     *  Example:
     *
     * \code{.gear}
     * // warning: out of range numeric literal
     * let x = 9999999999999999;
     * \endcode
     */
    GEAR_WARN_NUMERIC_TRUNCATION,

    /*! \brief Warns the user about empty block statements.
     *
     *  A warning is reported when an empty block scope is encountered.
     *  Block scopes begin with an open curly brace '{' and close with a closing curly brace '}'.
     *  Example:
     *
     * \code{.gear}
     * if x > y {
     *     // warning: empty block statement
     * }
     * \endcode
     */
    GEAR_WARN_EMPTY,

    /*! \brief Warns the user about unreachable statements.
     *
     *  Warns the user if a statement will never be executed.
     *  Example:
     *
     * \code{.gear}
     * func pi() -> Float {
     *     return 3.14;
     *     foo(); // warning: unreachable statement
     * }
     * \endcode
     */
    GEAR_WARN_UNREACHABLE,

    /*! \brief Warns the user about unknown attributes.
     *
     *  Warns the user if an object is declared with unknown attributes.
     *  Example:
     *
     * \code{.gear}
     * // warning: unknown attribute 'blah'
     * type Thing (blah) {
     * }
     * \endcode
     */
    GEAR_WARN_UNKNOWN_ATTRIBUTE,

    /*! \brief Warns if 'this' is assigned to
     *
     *  Warns the user if they attempt to assign to 'this' in a method.
     *  Assignment to 'this' is not an error, however, it is discouraged to avoid confusion.
     *  Example:
     *
     * \code{.gear}
     * type Frobnicator {
     *     func frobify(frob: Frobnicator) {
     *         // warning: assignment to 'this'
     *         this = frob;
     *     }
     * }
     * \endcode
     */
    GEAR_WARN_THIS_ASSIGNMENT,

    /*! \brief Warns the user declares variables, fields, or parameters named `this`
     *
     *  Naming variables, fields, or parameters 'this' can cause confusion.
     *  This warning encourages programmers to avoid this practice.
     *  Example:
     *
     * \code{.gear}
     * type Thingy {
     *     this: Int; // warning: field named 'this'
     * }
     * \endcode
     */
    GEAR_WARN_INVALID_THIS,

    /*! \brief Warns if the safe navigation operator is used on a non-nullable type.
     *
     *  Safe navigation (using '?.') is unnecessary on non-nullable types.
     *  This warning encourages using regular navigation (using '.').
     *  Example:
     *
     * \code{.gear}
     * func frobify(frob: Frob) {
     *     // warning: safe navigation on non-nullable type
     *     frob?.frobnicate();
     * }
     * \endcode
     */
    GEAR_WARN_USELESS_SAFE,

    /*! \brief Warns if function parameters are assigned a new value.
     *
     *  Encourages avoiding reassignment to function parameters.
     *  This warning is stylistic and not enabled by default.
     *  Example:
     *
     * \code{.gear}
     * func sum(x: Int, y: Int) -> Int {
     *     y = 42; // warning: parameter reassignment
     *     return x + y;
     * }
     * \endcode
     */
    GEAR_WARN_PARAM_REASSIGNMENT,

    /*! \brief Warns if a horizontal tab is found in the source code.
     *
     *  This warning is encourages spaces instead of tabs for indention.
     *  This warning is stylistic and not enabled by default.
     */
    GEAR_WARN_TABS,

    /*! \brief Warns if a variable shadows another variable.
     *
     *  This warning is encourages programmers to create unique variable names so they are not confused for another.
     *  This warning is stylistic and not enabled by default.
     */
    GEAR_WARN_VARIABLE_SHADOWING,

    /*! \brief Warns if a compilation unit contains inconsistent line endings.
     *
     *  This warning is intended to encourage programmers to use consistent line endings.
     *  This warning is stylistic and not enabled by default.
     */
    GEAR_WARN_MIXED_LINE_ENDINGS,

    /*! \brief Warns if a Yoda condition is used.
     *
     *  A Yoda condition is a conditional expression where the constant is on the left-hand side of the expression.
     *  It's named after the Star Wars character Yoda who speaks English with reverse syntax structure.
     *  Example:
     *
     * \code{.gear}
     * if 1 == x { // warning: yoda condition
     * }
     * \endcode
     * 
     *  This warning is stylistic and not enabled by default.
     */
    GEAR_WARN_YODA,

    /*! \brief A sentinel value for this enumeration.
     * 
     *  This will always be the last value of the enumeration.
     */
    GEAR_WARN_COUNT,
};

/*! \brief Defines the properties associated with a compilation unit.
 *
 *  The elements of this enumeration should be used in conjunction with the functions
 *  #gearc_unit_set_property and #gearc_unit_get_property to modify or retrieve the
 *  properties associated with a compilation unit.
 *
 *  \sa gearc_unit_set_property
 *  \sa gearc_unit_get_property
 */
enum gearc_unit_property
{
    /*! \brief Indicates if the compilation unit is the main entry point.
     *
     *  When a compilation unit is flagged as the main entry point, then no `main` function should be be provided.
     *  The top level statements of the compilation unit will be treated as if they were in a `main` function.
     *
     *  Only one compilation unit can be the main entry point.
     *  If more than one are flagged as an entry point, then the module will fail to compile.
     *
     *  Compilation units default to \b not being an entry point.
     *  Example:
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * gearc_unit *unit = gearc_unit_new(compiler);
     * gearc_unit_set_property(compiler, unit, GEAR_UNIT_MAIN, "1");
     * \endcode
     */
    GEAR_UNIT_MAIN,

    /*! \brief The name of the compilation unit.
     *
     *  Compilation units must have unique names to identify them.
     *  If the name is already in use by another unit, then attempting to change it with
     *  #gearc_unit_set_property will fail and a zero value will be returned.
     *  To help ensure uniqueness, if this unit is being created from a source file, then
     *  it's recommended to name it after the absolute file name and path.
     *  By default, compilation units are created with a unique name.
     *  Example:
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * gearc_unit *unit = gearc_unit_new(compiler);
     * gearc_unit_set_property(compiler, unit, GEAR_UNIT_NAME, "C:\\MyProject\\hello.gear");
     * \endcode
     */
    GEAR_UNIT_NAME,

    /*! \brief The name of the compilation unit as it appears in diagnostic messages.
     *
     *  The display name is the name the compilation unit will be referred to by diagnostic messages.
     *  This name does \b not have to be unique unlike #GEAR_UNIT_NAME.
     *  If a display name is not provided, then it defaults to #GEAR_UNIT_NAME.
     * 
     *  It is recommended to make this a shorter name, such as the file name without its path.
     *  Example:
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * gearc_unit *unit = gearc_unit_new(compiler);
     * gearc_unit_set_property(compiler, unit, GEAR_UNIT_NAME, "C:\\MyProjects\\AwesomeApp\\hello.gear");
     * gearc_unit_set_property(compiler, unit, GEAR_UNIT_DISPLAY_NAME, "hello.gear");
     * \endcode
     */
    GEAR_UNIT_DISPLAY_NAME,

    /*! \brief The source code of the compilation unit.
     *
     *  The Gear source code associated with the compilation unit.
     *  Compilation units default to an empty string.
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * gearc_unit *unit = gearc_unit_new(compiler);
     * gearc_unit_set_property(compiler, unit, GEAR_UNIT_SOURCE, "print(\"Hello, World!\");");
     * \endcode
     */
    GEAR_UNIT_SOURCE,
};

/*! \brief Defines the different output targets.
 *
 *  Each element of this enumeration defines the possible output targets of the Gear compiler.
 *
 *  \sa gearc_build
 */
enum gearc_target_type
{
    /*! \brief Application target.
     *
     *  An application target builds a Gear module with an entry point.
     *  The program can be loaded and run by the Gear runtime.
     *  An entry point must be provided by one of the source files otherwise the code will fail to build.
     *  Example:
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * // ... add compilation units ...
     * // ... compile them ...
     * gearc_build(compiler, GEAR_TARGET_APPLICATION, "MyApp");
     * \endcode
     */
    GEAR_TARGET_APPLICATION,

    /*! \brief Library target.
     *
     *  An library target builds a Gear module without an entry point.
     *  Libraries provide useful utilities or services to a Gear application.
     *  They cannot be executed by the Gear runtime by themselves.
     *  Example:
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * // ... add compilation units ...
     * // ... compile them ...
     * gearc_build(compiler, GEAR_TARGET_LIBRARY, "MyLib");
     * \endcode
     */
    GEAR_TARGET_LIBRARY,

    /*! \brief Test target.
     *
     *  A test target builds a Gear module for testing.
     *  It must not have an entry point.
     *  Example:
     *
     * \code{.c}
     * gear_compiler *compiler = gearc_new();
     * // ... add compilation units ...
     * // ... compile them ...
     * gearc_build(compiler, GEAR_TARGET_TEST, "MyTests");
     * \endcode
     */
    GEAR_TARGET_TEST,
};

/*! \brief The Gear compiler manages all data pertaining to interactions with the compiler library.
 *
 *  A Gear compiler is responsible for building Gear code.
 *  You can create as many Gear compiler instances as you like, however,
 *  a compiler should only be used on the thread that created it.
 *
 *  All public API operations require a pointer to a #gear_compiler.
 *
 *  \sa gearc_new
 *  \sa gearc_delete
 */
typedef struct gear_compiler gear_compiler;

/*! \brief A Gear compilation unit represents either a complete or partial Gear program.
 *
 *  One compilation unit typically corresponds to one source file.
 *
 *  \sa gearc_unit_new
 *  \sa gearc_unit_delete
 */
typedef struct gearc_unit gearc_unit;

/*! \brief Describes a single modification to a compilation units source code.
 */
typedef struct gearc_edit gearc_edit;

/*! \brief Allocates a new #gear_compiler.
 *
 *  A Gear compiler can build a runnable Gear program from one or more compilation units.
 *
 *  \return A heap allocated #gear_compiler.
 *          The runtime must be released with #gear_delete.
 *
 *  \sa gearc_delete
 *  \since 0.1.0
 */
GEAR_API gear_compiler *gearc_new(void);

/*! \brief Releases resources for a Gear compiler instance.
 *
 *  Once a compiler has been released it should not be used again.
 *
 *  \param[in] compiler An instance of the Gear compiler to free.
 *                      The pointer should have been returned by #gearc_new.
 *
 *  \since 0.1.0
 *  \sa gearc_new
 */
GEAR_API void gearc_delete(gear_compiler *compiler);

/*! \brief Enables or disables compiler warning(s).
 *
 *  The Gear compiler can emit warnings for possible errors, stylistic issues, and encourage best practices.
 *  All warnings except stylistic warnings are enabled by default.
 *
 *  \code
 *  // Enable a specific warning.
 *  gearc_warning_toggle(compiler, GEAR_WARN_UNUSED_PARAMS, 1);
 *
 *  // Disable a specific warning.
 *  gearc_warning_toggle(compiler, GEAR_WARN_UNUSED_PARAMS, 0);
 *
 *  // Enable all warnings.
 *  gearc_warning_toggle(compiler, GEAR_WARN_ALL, 1);
 *
 *  // Disable all warnings.
 *  gearc_warning_toggle(compiler, GEAR_WARN_ALL, 0);
 *  \endcode
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] warning The warning to enable or disable.
 *  \param[in] toggle Enables the warning if non-zero otherwise it disables the warning.
 *
 *  \sa gearc_warning_status
 *  \since 0.1.0
 */
GEAR_API void gearc_warning_toggle(gear_compiler *compiler, gear_warning_type warning, int toggle);

/*! \brief Checks if a warning is enabled or disabled.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] warning The warning to query.
 *
 *  \return A non-zero value is returned if the warning is enabled.
 *
 *  \sa gearc_warning_toggle
 *  \since 0.1.0
 */
GEAR_API int gearc_warning_status(gear_compiler *compiler, gear_warning_type warning);

/*! \brief Compiles all compilation units.
 *
 *  Compiles all compilation units and reports errors.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *
 *  \since 0.1.0
 */
GEAR_API void gearc_compile(gear_compiler *compiler);

/*! \brief Generates a Gear module.
 *
 *  The code must be compiled with #gearc_compile before this function is called.
 *  Once a compiler has been released it should not be used again.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] target The kind of target to generate (e.g. executable, library, etc...).
 *  \param[in] outfile The name of the file to write the compiled module to.
 *
 *  \sa gearc_compile
 *  \since 0.1.0
 */
GEAR_API void gearc_build(gear_compiler *compiler, gearc_target_type target, const char *outfile);

/*! \brief Allocates a new compilation unit.
 *
 *  The unit should be free'd with #gearc_unit_delete.
 *  Units not free'd will automatically be free'd when the Gear compiler is free'd with #gearc_delete.
 *
 *  Source code can be updated with #gearc_unit_update.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \return A heap allocated #gearc_unit.
 *
 *  \sa gearc_unit_delete
 *  \since 0.1.0
 */
GEAR_API gearc_unit *gearc_unit_new(gear_compiler *compiler);

/*! \brief Retrieves the compilation unit by name.
 *
 *  If the compilation unit doesn't exist, then NULL is returned.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] name The name of the compilation unit.
 *  \return A heap allocated #gearc_unit.
 *
 *  \sa gearc_unit_delete
 *  \since 0.1.0
 */
GEAR_API gearc_unit *gearc_unit_get(gear_compiler *compiler, const char *name);

/*! \brief Modifies a property of a compilation unit.
 *
 *  If the property is invalid, then NULL is returned.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] unit The compilation unit whose property will be retrieved.
 *  \param[in] proprety The property to retrieve.
 *  \param[in] value The new value of the property.
 *  \return A non-zero integer is returned on success.
 *  \since 0.1.0
 */
GEAR_API int gearc_unit_set_property(gear_compiler *compiler, gearc_unit *unit, gearc_unit_property proprety, const char *value);

/*! \brief Retrievers the property of a compilation unit.
 *
 *  If the property is invalid, then NULL is returned.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] unit The compilation unit whose property will be retrieved.
 *  \param[in] proprety The property to retrieve.
 *  \return The value of the given property.
 *  \since 0.1.0
 */
GEAR_API const char *gearc_unit_get_property(gear_compiler *compiler, gearc_unit *unit, gearc_unit_property proprety);

/*! \brief Free's a compilation unit.
 *
 *  Units not free'd will automatically be free'd when the Gear compiler is free'd with #gearc_delete.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] unit The compilation unit to delete.
 *
 *  \sa gearc_unit_new
 *  \since 0.1.0
 */
GEAR_API void gearc_unit_delete(gear_compiler *compiler, gearc_unit *unit);

/*! \brief Modifies the source code of a compilation unit.
 *
 *  The unit should be free'd with #gearc_unit_delete.
 *  Units not free'd will automatically be free'd when the Gear compiler is free'd with #gearc_delete.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] unit The compilation unit to delete.
 *  \param[in] edits The edits to apply.
 *  \param[in] edit_count The number of edits to apply.
 *
 *  \sa gearc_edit_add
 *  \sa gearc_edit_delete
 *  \since 0.1.0
 */
GEAR_API void gearc_unit_edit(gear_compiler *compiler, gearc_unit *unit, gearc_edit **edits, int edit_count);

/*! \brief Creates an edit representing the addition of a single line of source code.
 *
 *  This edit represents the addition of a single line of source code.
 *  The source code will be inserted after the given line number.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] line The line number to insert the content afterwards.
 *  \param[in] content Gear source code.
 *
 *  \return A non-zero integer is returned if the compilation unit was successfully renamed.
 *
 *  \sa gearc_unit_edit
 *  \since 0.1.0
 */
GEAR_API gearc_edit *gearc_edit_add(gear_compiler *compiler, int line, const char *content);

/*! \brief Creates an edit representing the deletion of a single line of source code.
 *
 *  This edit represents the deletion of a single line of source code.
 *
 *  \param[in] compiler An instance of the Gear compiler.
 *  \param[in] line The line number being deleted.
 *
 *  \return A non-zero integer is returned if the compilation unit was successfully renamed.
 *
 *  \sa gearc_unit_edit
 *  \since 0.1.0
 */
GEAR_API gearc_edit *gearc_edit_delete(gear_compiler *compiler, int line);

#endif
