/*! \file gear_runtime.h
 *  \brief This header defines the Gear Runtime API.
 *
 *  This header defines the API for managing a Gear runtime.
 *  Consumers of this header must link against `gear.lib` on Windows and `libgear.a` on *nix systems.
 */

#ifndef GEAR_RUNTIME_API_H
#define GEAR_RUNTIME_API_H

#include "gear_config.h"

/*! \brief Represents an instance of a Gear runtime.
 *
 *  A Gear runtime is responsible for managing the virtual machine and garbage collector.
 *  There are no limits on how many runtime's can be created and they are entirely self contained.
 *
 *  All public API operations in this header require a pointer to a #gear_runtime.
 *
 *  \sa gear_new_from_file
 *  \sa gear_new_from_memory
 *  \sa gear_delete
 */
typedef struct gear_runtime gear_runtime;

/*! \brief A virtual machine register for storing a Gear value.
 *
 *  This is an abstract handle representing a virtual machine register.
 *  A register can be thought of as a variable that references a Gear value.
 *  They are useful for exchanging data between a host and a runtime.
 *
 *  This type should be treated as an *abstract handle*.
 *  Its underlying integer value is not safe to inspect or modify.
 *  For instance, register assignment should be performed using #gear_move of C's assignment operator.
 *
 *  While a value is referenced by a register, it cannot be garbage collected.
 *  There is no limit on the number of virtual registers that can be created.
 */
typedef long gear_register;

/*! \brief The function prototype of a C function that's exposed to Gear.
 *
 *  Natively wrapped C functions should conform to this function signature.
 */
typedef int(*gear_C_function)(gear_runtime *runtime, int argc);

/*! \brief The signature for a C function 
 *
 *  Natively wrapped C functions should conform to this function signature.
 *
 *  There are two methods of catching errors:
 *  Calling #gear_get_last_error after a call to an API or
 *  registering a callback to be invoked when an error occurs.
 *
 *  All errors go through both the callback and #gear_get_last_error.
 *  If you use one, but not the other you won't miss any errors.
 */
typedef int(*gear_C_error)(const char *error_message);

/*! \brief Allocates a new #gear_runtime from a compiled Gear program file.
 *
 *  Opens the specified file as a Gear program and returns a new instance of a #gear_runtime.
 *  The runtime must be released with #gear_delete to prevent resource leakage.
 *
 *  \param[in] file_name The name of the compiled Gear program file.
 *  \return A heap allocated #gear_runtime.
 *          The runtime must be released with #gear_delete.
 *
 *  \sa gear_new_from_memory
 *  \sa gear_delete
 *  \since 0.1.0
 */
GEAR_API gear_runtime *gear_new_from_file(const char *file_name);

/*! \brief Allocates a new #gear_runtime 
 *
 *  The runtime returned by this function must be released with #gear_delete.
 *  The runtime must be released with #gear_delete to prevent resource leakage.
 *
 *  \param[in] buffer A byte buffer of a Gear program file.
 *  \param[in] buffer_size The size of the buffer.
 *  \return A instance of a #gear_runtime.
 *
 *  \sa gear_new_from_file
 *  \sa gear_delete
 *  \since 0.1.0
 */
GEAR_API gear_runtime *gear_new_from_memory(const unsigned char *buffer, int buffer_size);

/*! \brief Releases resources associated with a Gear runtime.
 *
 *  Once a runtime has been released it should not be used again.
 *  Any runtime resources, like allocated registers, will automatically be free'd.
 *
 *  \param[in] runtime The Gear runtime to release.
 *                     The runtime should have been generated by #gear_new_from_file or #gear_new_from_memory.
 *
 *  \since 0.1.0
 *  \sa gear_new_from_file
 *  \sa gear_new_from_memory
 */
GEAR_API void gear_delete(gear_runtime *runtime);

/*! \brief Allocates N number of registers. 
 *
 *  Registers allocated with this function must be freed with a call to #gear_free_registers.
 *
 * \code
 * gear_register reg;
 * gear_register regs[5];
 *
 * gear_alloc_registers(runtime, 1, &reg); // Reserve one register.
 * gear_alloc_registers(runtime, 5, regs); // Reserve multiple registers.
 *
 * // ... do something with the registers ...
 *
 * // Free all registers.
 * gear_free_registers(runtime, 1, &reg);
 * gear_free_registers(runtime, 5, regs);
 * \endcode
 * 
 *  There is no limit on the number of virtual registers that can be created.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] n The number of registers to allocate.
 *  \param[out] regs An array in which the allocated registers are stored.
 *
 *  \since 0.1.0
 *  \sa gear_free_registers
 */
GEAR_API void gear_alloc_registers(gear_runtime *runtime, int n, gear_register *regs);

/*! \brief Deletes N number of registers.
 *
 *  Deletes registers allocated with #gear_alloc_registers.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] n The number of registers to free.
 *  \param[in] regs The registers to free.
 *
 *  \since 0.1.0
 *  \sa gear_alloc_registers
 */
GEAR_API void gear_free_registers(gear_runtime *runtime, int n, gear_register *regs);

/*! \brief Provides an implementation for a native function.
 *
 *  All native functions must be implemented.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] name The name of the exported function to invoke.
 *  \param[in] function The native function.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_implement_function(gear_runtime *runtime, const char *name, gear_C_function function);

/*! \brief Loads a function into the specified register.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to place the function in.
 *  \param[in] name The name of the function.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_load_function(struct gear_runtime *runtime, gear_register reg, const char *name);

/*! \brief Calls a function with zero or more arguments.
 *
 *  This function will null out all parameter registers so you don't have to.
 *  It will override the last value in the return result register.
 *
 * \code
 * // Allocate a register to hold the "add" function.
 * gear_register addfunc;
 * gear_alloc_registers(runtime, 1, &addfunc);
 * gear_get_symbol(runtime, "add", addfunc);
 * 
 * // Call the "add" function with two arguments.
 * gear_set_int(runtime, GEAR_PARAM(0), 4); // Set the first argument to the integer 4.
 * gear_set_int(runtime, GEAR_PARAM(1), 5); // Set the second argument to the integer 5.
 * gear_call(runtime, addfunc, 2);          // Invoke the "add" function with 2 arguments.
 *
 * // Print the result.
 * int result = gear_get_int(runtime, GEAR_RETURN);
 * printf("4 + 5 = %d", result);
 * \endcode
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] name The name of the exported function to invoke.
 *  \param[in] argc The number of arguments to invoke the function with.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_call(struct gear_runtime* runtime, gear_register reg, int argc);

/*! \brief Calls a function with zero or more arguments by its string name.
 *
 *  This function will null out all parameter registers so you don't have to.
 *  It will override the last value in the return result register.
 *
 * \code
 * // Call the "add" function with two arguments.
 * gear_set_int(runtime, GEAR_PARAM(0), 4); // Set the first argument to the integer 4.
 * gear_set_int(runtime, GEAR_PARAM(1), 5); // Set the second argument to the integer 5.
 * gear_call_by_name(runtime, "add", 2);            // Invoke the "add" function with 2 arguments.
 *
 * // Print the result.
 * int result = gear_get_int(runtime, GEAR_RETURN);
 * printf("4 + 5 = %d", result);
 * \endcode
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] name The name of the exported function to invoke.
 *  \param[in] argc The number of arguments to invoke the function with.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_call_by_name(gear_runtime* runtime, const char* name, int argc);

/*! \brief Moves the value from one register to another.
 *
 *  The value will still be referenced by the original register
 *  until a new value overrides it or it's cleared with #gear_set_null.
 *  Primitive types are copied and composite types are not.
 * 
 *  \note
 *  Values will be copied if they are value types.
 *  Reference types will *not* be copied and the destination register will simply contain another reference.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] src The source register to move the value from.
 *  \param[in] dest The destination register to move the value to.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_move(gear_runtime *runtime, gear_register src, gear_register dest);

/*! \brief Retrieves a symbol by its name and stores it in the specified register.
 *
 *  Symbols can be anything: Variables, functions, types, etc...
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] symbol The symbol whose value is retrieved.
 *  \param[in] dest The destination register.
 *  \return A non-zero value is returned if the symbol could not be found.
 *
 *  \since 0.1.0
 */
GEAR_API int gear_get_symbol(gear_runtime *runtime, const char *symbol, gear_register dest);

/*! \brief Returns true if the value contained in the register is null.
 *
 *  Determines if the register contains a null value.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register.
 *
 *  \return Non-zero if the register contains null.
 *
 *  \since 0.1.0
 *  \sa gear_set_null
 */
GEAR_API int gear_is_null(gear_runtime *runtime, gear_register reg);

/*! \brief Sets the value of the register to null.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to make null.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_null(gear_runtime *runtime, gear_register reg);

/*! \brief Sets the value of the register to an integer.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to set.
 *  \param[in] value The integer value to store in the register.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_int(gear_runtime *runtime, gear_register reg, gear_int value);

/*! \brief Sets the value of the register to a float.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to set.
 *  \param[in] value The integer value to store in the register.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_float(gear_runtime *runtime, gear_register r, gear_float value);

/*! \brief Sets the value of the register to a boolean.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to set.
 *  \param[in] value The integer value to store in the register.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_bool(gear_runtime *runtime, gear_register r, int value);

/*! \brief Sets the value of the register to a string.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to set.
 *  \param[in] value The integer value to store in the register.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_string(gear_runtime *runtime, gear_register r, const char *value);

/*! \brief Sets the value of the register to a C function.
 *
 *  A function can be passed to the runtime for invocation.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to set.
 *  \param[in] function The function to store in the register.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_function(gear_runtime *runtime, gear_register reg, gear_C_function function);
GEAR_API void gear_set_field_function(gear_runtime *runtime, gear_register register_num, const char *field_name, gear_C_function function);

/*! \brief Allocates a new object and assigns it to the specified register.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to be assigned the object.
 *  \param[in] symbol The exported symbol name of the type to allocate.
 *
 *  \since 0.1.0
 */
GEAR_API void gear_set_object(gear_runtime *runtime, gear_register reg, const char *symbol);

/*! \brief Returns the integer value stored in a register.
 *
 *  If the register stores a float, then its value is converted to an integer and returned.
 *  If it stores a boolean, then its value is convert to the integer 0 (for false) or 1 (for true).
 *  For all other types 0 is returned and the error flag set.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to read from.
 *
 *  \return The value of the register as an integer.
 *
 *  \since 0.1.0
 */
GEAR_API gear_int gear_get_int(gear_runtime *runtime, gear_register reg);

/*! \brief Returns the float value stored in a register.
 *
 *  If the register stores an integer, then its value is converted to a float and returned.
 *  For all other types 0 is returned and the error flag set.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to read from.
 *
 *  \return The value of the register as a float.
 *
 *  \since 0.1.0
 */
GEAR_API gear_float gear_get_float(gear_runtime *runtime, gear_register reg);

/*! \brief Returns the boolean value stored in a register.
 *
 *  All values are truthy except for false, null, and the integer zero.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to read from.
 *  \return The value of the register as a boolean (the integer 0 or 1).
 *  \since 0.1.0
 */
GEAR_API int gear_get_bool(gear_runtime *runtime, gear_register reg);

/*! \brief Returns the string value stored in a register.
 *
 *  If the register does not store a string, then the value will be converted to a string.
 *  If the value is an object, then its type name is returned.
 *
 *  This function will not invoke the objects \b toString method.
 *  You must invoke it manually and handle any exceptions that occur.
 *
 *  \param[in] runtime The Gear runtime.
 *  \param[in] reg The register to read from.
 *
 *  \return The value of the register as a string.
 *
 *  \since 0.1.0
 */
GEAR_API const char *gear_get_string(gear_runtime *runtime, gear_register reg);

/*! \brief Registers a callback function to invoke on an error.
  *
  *  A breakpoint can be placed within the callback to catch the moment an error occurs.
  *  This can be useful for inspecting a stacktrace a quickly determining the cause of the error.
  *  Passing NULL removes the callback from the runtime.
  *
  *  \param[in] runtime The Gear runtime.
  *  \param[in] callback A function to invoke when an error occurs.
  *
  *  \since 0.1.0
 */
GEAR_API void gear_set_error_callback(gear_runtime *runtime, gear_C_error callback);

/*! \brief Returns a description of the last error that occurred.
 *
 *  Returns a description of the last error that occurred.
 *  If no error occurred, then NULL is returned.
 *
 *  Calling this function clears the error so subsequent calls will return NULL.
 *  The string should be copied or used immediately before the next call to this function.
 *
 *  \param[in] runtime The Gear runtime.
 *
 *  \return A string describing the error.
 *
 *  \since 0.1.0
 */
GEAR_API const char *gear_get_last_error(gear_runtime *runtime);

#endif
