// [...]

// EXTENSION 2019-07-24

/**
 * Primitives Interrupt Types declaration for the User Module.
 */

/**
 * extern "C" bool bpattach(natl id, vaddr rip);
 */
#define  TIPO_BPA  0x59

/**
 * extern "C" void bpwait();
 */
#define  TIPO_BPW  0x5a

/**
 * extern "C" void bpdetach();
 */
#define  TIPO_BPD  0x5b

// EXTENSION 2019-07-24

// [...]

