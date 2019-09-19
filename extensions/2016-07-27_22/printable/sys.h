// EXTENSION 2016-07-27

/**
 * This user primitive can be used to retrieve into the addressed buffer 'buf'
 * a maximum number of specified bytes (quanti) - as the CE device might contain
 * more available bytes which will therefore be lost - from the CE device having
 * the given id. It must check for the given buffer address to be aligned to the
 * page, for 0 <= quanti <= 10*pages. In case of error the primitive must abort
 * the calling process.
 *
 * @param  id      CE device id;
 * @param  quanti  number of bytes to be transferred;
 * @param  buf     destination buffer address;
 *
 * @return  true if the given buffer was big enough for all available bytes,
 *          false otherwise.
 */
extern "C" bool cedmaread(natl id, natl& quanti, char *buf);
extern "C" bool cedmaread2(natl id, natl* quanti, char *buf);

// EXTENSION 2016-07-27

