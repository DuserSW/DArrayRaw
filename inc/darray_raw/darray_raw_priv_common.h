#ifndef DARRAY_RAW_PRIV_COMMON_H
#define DARRAY_RAW_PRIV_COMMON_H


/*
    This is the private header for DArrayRaw library. Do not include it directly!


    Author: Kamil Kielbasa
    Email: kamilkielbasa64@gmail.com
    License: GPL3
*/


#include <stdint.h>
#include <string.h>


/* typedef for comparator function */
typedef int (*compare_fp)(const void*, const void*);


/* typedef for destructor function */
typedef void (*destructor_fp)(void*);


/* functionlike macro for getting length of arrays allocated on stack */
#define array_size(array) (sizeof(array) / sizeof((array)[0]))


/*
 * Functionlike macro which copies @size_of bytes from the object pointed to by @src_p to the object pointed to by @dst_p. 
 * If the objects overlap, the behavior is undefined. If either @dst_p or @src_p is an invalid or null pointer, the behavior 
 * is undefined, even if @size_of bytes is zero. 
 * 
 * Functionlike macro introduce optimization if we copy objects with sizeof range [1, 4]. 
 * Function memcpy call is not necessary because we can just "assign" these objetcs.
 *
 * @param[in] dst_p   - pointer to array.
 * @param[in] src_p   - size of each array member.
 * @param[in] size_of - number of bytes to copy.
 * 
 * @return nothing.
 */
#define assign(dst_p, src_p, size_of) \
    do { \
        register void* const restrict internal_dst_p = dst_p; \
        register const void* const restrict internal_src_p = src_p; \
        switch (size_of) \
        { \
            case sizeof(uint8_t):  *(uint8_t * const restrict)internal_dst_p = *(const uint8_t * const restrict)internal_src_p; break; \
            case sizeof(uint16_t): *(uint16_t* const restrict)internal_dst_p = *(const uint16_t* const restrict)internal_src_p; break; \
            case sizeof(uint32_t): *(uint32_t* const restrict)internal_dst_p = *(const uint32_t* const restrict)internal_src_p; break; \
            case sizeof(uint64_t): *(uint64_t* const restrict)internal_dst_p = *(const uint64_t* const restrict)internal_src_p; break; \
            default: (void)memcpy(internal_dst_p, internal_src_p, size_of); break; \
        } \
    } while (0)


/* 
 * Functionlike macro for change values (swap) of two object.
 *
 * @param[in] first_addr_p  - address to first object.
 * @param[in] second_addr_p - address to second object.
 * @param[in] size_of       - number of bytes of any object.
 * 
 * @return nothing.
 */
#define swap(first_addr_p, second_addr_p, size_of) \
    do { \
        uint8_t vla[size_of]; \
        assign(&vla[0], first_addr_p, size_of); \
        assign(first_addr_p, second_addr_p, size_of); \
        assign(second_addr_p, &vla[0], size_of); \
    } while (0)


#endif /* DARRAY_RAW_PRIV_COMMON_H */
