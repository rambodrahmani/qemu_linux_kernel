
// EXTENSION 2017-02-24

/**
 *
 */
extern "C" natq countres();

// EXTENSION 2017-02-24

// EXTENSION 2017-02-24

/**
 * Reads the required amount of bytes from the specified CE device.
 *
 * @param  id      CE device ID;
 * @param  quanti  number of bytes to be transferred;
 * @param  buf     destination buffer virtual address.
 */
extern "C" bool cedmaread(natl id, natl quanti, char *buf);

// EXTENSION 2017-02-24

