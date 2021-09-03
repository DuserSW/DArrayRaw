#ifndef DARRAY_RAW_H
#define DARRAY_RAW_H


#include "darray_raw_priv_common.h"


/*
    This is the main header for DArrayRaw library.


    Author: Kamil Kielbasa
    Email: kamilkielbasa64@gmail.com
    License: GPL3


    Main features - library is standalone and wrap or implement basic operation on arrays like:
    * create/destroy.
    * copy/clone/move/zeros/set_all.
    * ...
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


/*
 * Function copy N bytes @src_array_p into @dst_array_p. the memory areas may not overlap. 
 * Number of copied bytes are calculated from @size_of multiply @length.
 *
 * @param[in] dst_array_p - pointer to destination array.
 * @param[in] src_array_p - pointer to source array.
 * @param[in] size_of     - size of each array member.
 * @param[in] length      - number of elements in array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_copy(void* restrict dst_array_p, void* restrict src_array_p, size_t size_of, size_t length);


/*
 * Function clone array @array_p based on size calculated from @size_of multiply @length.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: cloned array on success, NULL on failure.
 */
void* darray_raw_clone(void* array_p, size_t size_of, size_t length);


/*
 * Function copy N bytes from @src_array_p into @dst_array_p. The memory areas may overlap. 
 * Number of copied bytes are calculated from @size_of multiply @length.
 *
 * @param[in] dst_array_p - pointer to destination array.
 * @param[in] src_array_p - pointer to source array.
 * @param[in] size_of     - size of each array member.
 * @param[in] length      - number of elements in array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_move(void* dst_array_p, void* src_array_p, size_t size_of, size_t length);


/*
 * Function set N bytes from @array_p to zero value.
 * Number of set bytes are calculated from @size_of multiply @length.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_zeros(void* array_p, size_t size_of, size_t length);


/*
 * Function set N bytes from @array_p to @val value.
 * Number of set bytes are calculated from @size_of multiply @length.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] data_p  - constant data which fill array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_set_all(void* restrict array_p, size_t size_of, size_t length, const void* restrict data_p);


#endif /* DARRAY_RAW_H */
