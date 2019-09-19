// EXTENSION 2016-07-06

/**
 * Declare primitive here in order for it to be accessible from the user module.
 *
 * This primitive reads from the RBR register of the CE device having the given
 * id into the provided buffer until the char 'stop' is retrieved or the maximum
 * number of bytes is reached. 
 *
 * @param  id      CE device id;
 * @param  buf     destination buffer address;
 * @param  quanti  number of bytes to retrieve/number of bytes actually
 *                 transferred;
 * @param  stop    stop char.
 */
extern "C" void ceread(natl id, char *buf, natl& quanti, char stop);

// EXTENSION 2016-07-06

