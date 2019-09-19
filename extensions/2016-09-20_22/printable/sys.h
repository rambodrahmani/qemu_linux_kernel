// EXTENSION 2016-09-20

/**
 * We want to add a broadcasting mechanism to the system allowing a process to
 * send a broadcast messages to all of the registered listener processes. To do
 * it we provide the reg() primitive which can be used to register a process
 * as a listener, the listen() primitive which can be used to receive the
 * broadcast message and broadcast(natl msg) which can be used by the
 * broadcaster process to send a broadcast message.
 */

/**
 * PRIMITIVES DECLARATIONS.
 */

/**
 * Registers the process as a listener of broadcast messages. No action will
 * take place if the process is already a registered listener. We will be using
 * a global broadcast descriptor and all processes will be registered as
 * listener of this global broadcast descriptor.
 */
extern "C" void reg();

/**
 * Waits for the next broadcast message. An error should be rised if the process
 * is not registered as a listener of the broadcast messages. We will be using
 * a global broadcast descriptor and this primitive can be used to wait for the
 * next message sent on the global broadcast descriptor.
 */
extern "C" natl listen();

/**
 * Broadcasts the given message to all registered processes. An error should be
 * rised if the process is not registered as a listener of the broadcast
 * messages. We will be using a global broadcast descriptor for simplicity and
 * the broadcaster register can use this primitive to send a new message on the
 * global broadcast.
 *
 * @param  msg  the message to be broadcasted.
 */
extern "C" void broadcast(natl msg);

// EXTENSION 2016-09-20

