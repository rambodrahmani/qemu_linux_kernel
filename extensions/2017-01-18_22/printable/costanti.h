// EXTENSION 2017-01-18

/**
 * Maximum number of broadcast messages. Once this value is reached any attempt
 * of sending a broadcast message will result in the calling process being
 * aborted.
 */
#define MAX_BROADCAST		50

// EXTENSION 2017-01-18

// EXTENSION 2016-09-20

/**
 * Interrupts types definitions for user primitives for the broadcast mechanism.
 * In this implementation we will be taking one step further the one provided in
 * 2016-09-20_22.
 */
#define TIPO_R			0x3a	// void reg(broadcast_role role)
#define TIPO_LS			0x3b	// natl listen()
#define TIPO_B			0x3c	// void broadcast(natl msg)

// EXTENSION 2016-09-20

