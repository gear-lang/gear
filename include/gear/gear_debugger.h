/*! \file gear_debugger.h
 *  \brief This header defines the Debugger API.
 *
 *  The debugger API consists of functions for starting and stopping a debug server on a Gear runtime.
 *  When a debug server is running, the session can be connected to with a remote debugger \e client.
 *  The client can issue commands, such as setting breakpoints and evaluating expressions.
 *
 *  \attention
 *  Consumers of this header must link against `geardbg.lib` on Windows or `libgeardbg.a`
 *  on *nix systems otherwise they will receive missing symbol errors at program link time.
 *
 *  \note
 *  The debug server implements the *Debug Adapter Protocol* which is defined here:
 *  https://microsoft.github.io/debug-adapter-protocol/.
 */

#ifndef GEAR_DEBUGGER_API_H
#define GEAR_DEBUGGER_API_H

#include "gear_config.h"
#include "gear_runtime.h"

/*! \brief Starts a debug server on the given Gear runtime.
 *
 *  This function starts a debug server on the specified address and port for the given Gear runtime.
 *  It can optionally be started in a blocking state which will cause the current process to halt until
 *  a remote debugger connects to it.
 *  This is useful for short running applications, like utility scripts, because without blocking
 *  they would run and terminate before a debugger has had a chance to connect.
 *  Applications that require user intervention to terminate, like GUI applications, should prefer
 *  to start in a non-blocking state to keep the application responsive.
 *
 *  The default address and port of the Gear remote debugger client is \b 0.0.0.0 on port \b 9229.
 *  If the debug server is started on a different address or port, than the client should be started with
 *  the same options otherwise it will fail to connect.
 *  Consult the documentation of the remote debugger for more details.
 * 
 *  \attention
 *  An error will be returned if a debug server is already active for the given Gear runtime or if
 *  the server socket cannot be created on the given address or port.
 *
 *  \note
 *  The debug server and client are implemented using WinSock2 (on Windows) and Berkeley Sockets (on macOS and Linux).
 *  No other communication protocols are supported at this time.
 *
 *  \param[in] runtime The runtime instance the debug server is created for.
 *  \param[in] address The network address to start the listening server on.
 *  \param[in] port The port to start the server on.
 *  \param[in] wait Indicates if the process should block until a remote debugger attaches itself to the server.
 *  \return A non-zero value is returned on error.
 *  \since 0.1.0
 */
GEAR_API int gear_start_debug_server(gear_runtime *runtime, const char *address, int port, bool wait);

/*! \brief Stops a debug server running on the given Gear runtime.
 *
 *  If no debug server is running, then this function performs no action.
 *  If the the debug server is not stopped manually by calling this function, then
 *  it will be stopped automatically when the Gear runtime is released with #gear_delete.
 *
 *  \param[in] runtime The Gear runtime to stop the debug server on.
 *  \since 0.1.0
 */
GEAR_API void gear_stop_debug_server(gear_runtime *runtime);

#endif
