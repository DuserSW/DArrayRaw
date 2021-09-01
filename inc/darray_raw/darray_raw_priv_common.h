#ifndef DARRAY_RAW_PRIV_COMMON_H
#define DARRAY_RAW_PRIV_COMMON_H


/*
    This is the private header for DArrayRaw library. Do not include it directly!


    Author: Kamil Kielbasa
    Email: kamilkielbasa64@gmail.com
    License: GPL3
*/


/* typedef for comparator function */
typedef int (*compare_fp)(const void*, const void*);


/* typedef for destructor function */
typedef void (*destructor_fp)(void*);


/* functionlike macro for getting length of arrays allocated on stack */
#define array_size(array) (sizeof(array) / sizeof((array)[0]))


#endif /* DARRAY_RAW_PRIV_COMMON_H */
