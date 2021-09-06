#ifndef DARRAY_RAW_PRIV_COMMON_H
#define DARRAY_RAW_PRIV_COMMON_H


/*
    This is the private header for DArrayRaw library. Do not include it directly!


    Author: Kamil Kielbasa
    Email: kamilkielbasa64@gmail.com
    License: GPL3
*/


#include <stdint.h>


/* typedef for comparator function */
typedef int (*compare_fp)(const void*, const void*);


/* typedef for destructor function */
typedef void (*destructor_fp)(void*);


/* functionlike macro for getting length of arrays allocated on stack */
#define array_size(array) (sizeof(array) / sizeof((array)[0]))


/* functionlike macro for change values of two variables. */
#define swap(a, b, size_of) \
    do { \
        uint8_t vla[size_of]; \
        memcpy(&vla[0], &a, size_of); \
        memcpy(&a, &b, size_of); \
        memcpy(&b, &vla[0], size_of); \
    } while (0)


#endif /* DARRAY_RAW_PRIV_COMMON_H */
