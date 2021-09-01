#ifndef DARRAY_RAW_H
#define DARRAY_RAW_H


#include "darray_raw_priv_common.h"


/*
    This is the main header for DArrayRaw library.


    Author: Kamil Kielbasa
    Email: kamilkielbasa64@gmail.com
    License: GPL3


    Main features:
    - ...
*/


#include <stddef.h>


/*
 * Function allocate memory for array on heap. By default array is zeros.
 *
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: allocated array on success, NULL on failure.
 */
void* darray_raw_create(size_t size_of, size_t length);


/*
 * Function deallocate memory under @array_p address.
 *
 * @param[in] array_p - pointer to array.
 * 
 * @return: this is void function.
 */
void darray_raw_destroy(void* array_p);


/*
 * Function deallocate memory under @array_p address. Additionally calls destructor on each of array member.
 *
 * @param[in] array_p    - pointer to array.
 * @param[in] size_of    - size of each array member.
 * @param[in] length     - number of elements in array.
 * @param[in] destroy_fp - pointer to destructor function.
 * 
 * @return: this is void function.
 */
void darray_raw_destroy_with_entires(void* array_p, size_t size_of, size_t length, const destructor_fp destroy_fp);


#endif /* DARRAY_RAW_H */
