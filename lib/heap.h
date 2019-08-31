/**
 * File: heap.h
 *       The heap is a region of your computer's memory that is not managed
 *       automatically for you, and is not as tightly managed by the CPU. It is
 *       a more free-floating region of memory (and is larger). To allocate
 *       memory on the heap, you must use malloc() or calloc(), which are
 *       built-in C functions. Once you have allocated memory on the heap, you
 *       are responsible for using free() to deallocate that memory once you
 *       don't need it any more. If you fail to do this, your program will have
 *       what is known as a memory leak. That is, memory on the heap will still
 *       be set aside (and won't be available to other processes).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 * Heap memory space descriptor. Identifies a space inside the system heap
 * memory.
 */
struct des_mem
{
    size_t size;
    des_mem* next;
};

/**
 * The heap memory area is made up of blocks (free and occupied). Each block of
 * memory is identified by and address and a size. Each block contains in its
 * initial bytes a block descriptor (des_mem) made up of two variables:
 *  1. size: the size in bytes of the block (the descriptor is not considered);
 *  2. next: meaningful only if the block is free, in which case it contains a
 *           pointer to the next free heap memory block.
 * The heap memory space can therefore be thought of as a list of free memory
 * blocks which top is pointed by free_heap.
 */
extern des_mem* free_heap;

/**
 *
 * @param  address
 * @param  size
 */
void internal_free(addr address, size_t size);

