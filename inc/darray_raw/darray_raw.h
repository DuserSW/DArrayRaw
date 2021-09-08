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
    * insert first/last/pos for unsorted arrays and insert for sorted arrays.
    * delete first/last/pos with/without entry for arrays.
    * check if array is sorted/reverse sorted/equals.
*/


#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>


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


/*
 * Function insert @data_p at first position (index: 0) of @array_p for unsorted array.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] data_p  - constant data which fill array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_unsorted_insert_first(void* restrict array_p, size_t size_of, size_t length, const void* restrict data_p);


/*
 * Function insert @data_p at last position (index: @length - 1) of @array_p for unsorted array.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] data_p  - constant data which fill array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_unsorted_insert_last(void* restrict array_p, size_t size_of, size_t length, const void* restrict data_p);


/*
 * Function insert @data_p at @pos of @array_p for unsorted array.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] pos     - array index to insert data.
 * @param[in] data_p  - constant data which fill array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_unsorted_insert_pos(void* restrict array_p, size_t size_of, size_t length, size_t pos, const void* restrict data_p);


/*
 * Function insert @data_p into @array_p for sorted array.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] data_p  - constant data which fill array.
 * @param[in] cmp_fp  - comparator function pointer.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_sorted_insert(void* restrict array_p, size_t size_of, size_t length, const void* restrict data_p, const compare_fp cmp_fp);


/*
 * Function delete first item from @array_p and zeros item from (@length - 1).
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_first(void* array_p, size_t size_of, size_t length);


/*
 * Function delete last item from @array_p and zeros item from (@length - 1).
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_last(void* array_p, size_t size_of, size_t length);


/*
 * Function delete item under @pos from @array_p and zeros item from (@length - 1).
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] pos     - array index to insert data.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_pos(void* array_p, size_t size_of, size_t length, size_t pos);


/*
 * Function delete all items from @array_p and zeros them.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_all(void* array_p, size_t size_of, size_t length);


/*
 * Function delete first item from @array_p, call destructor on item and zeros item from (@length - 1).
 *
 * @param[in] array_p    - pointer to array.
 * @param[in] size_of    - size of each array member.
 * @param[in] length     - number of elements in array.
 * @param[in] destroy_fp - destructor function pointer.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_first_with_entry(void* array_p, size_t size_of, size_t length, const destructor_fp destroy_fp);


/*
 * Function delete last item from @array_p, call destructor on item and zeros item from (@length - 1).
 *
 * @param[in] array_p    - pointer to array.
 * @param[in] size_of    - size of each array member.
 * @param[in] length     - number of elements in array.
 * @param[in] destroy_fp - destructor function pointer.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_last_with_entry(void* array_p, size_t size_of, size_t length, const destructor_fp destroy_fp);


/*
 * Function delete item under @pos from @array_p, call destructor on item and zeros item from (@length - 1).
 *
 * @param[in] array_p    - pointer to array.
 * @param[in] size_of    - size of each array member.
 * @param[in] length     - number of elements in array.
 * @param[in] pos        - array index to insert data.
 * @param[in] destroy_fp - destructor function pointer.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_pos_with_entry(void* array_p, size_t size_of, size_t length, size_t pos, const destructor_fp destroy_fp);


/*
 * Function delete all items from @array_p, call destructor on each of items and zeros them.
 *
 * @param[in] array_p    - pointer to array.
 * @param[in] size_of    - size of each array member.
 * @param[in] length     - number of elements in array.
 * @param[in] destroy_fp - destructor function pointer.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
int darray_raw_delete_all_with_entries(void* array_p, size_t size_of, size_t length, const destructor_fp destroy_fp);


/*
 * Function get upper bound of @data_p from @array_p.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] data_p  - constant data which fill array.
 * @param[in] cmp_fp  - comparator function pointer.
 * 
 * @return: upper bound index on success, -1 value on failure.
 */
ssize_t darray_raw_upper_bound(const void* restrict array_p, size_t size_of, size_t length, const void* restrict data_p, const compare_fp cmp_fp);


/*
 * Function shuffle @array_p.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 *
 * @return: this is void function.
 */
void darray_raw_shuffle(void* array_p, size_t size_of, size_t length);


/*
 * Function check if @first_array_p and @second_array_p are equals.
 *
 * @param[in] first_array_p  - pointer to first array to compare.
 * @param[in] second_array_p - pointer to second array to compare.
 * @param[in] size_of        - size of each array member.
 * @param[in] length         - number of elements in array.
 * @param[in] cmp_fp         - comparator function pointer.
 *
 * @return: true if @first_array_p and @second_array_p are equals, false if not.
 */
bool darray_raw_equal(const void* first_array_p, const void* second_array_p, size_t size_of, size_t length, const compare_fp cmp_fp);


/*
 * Function check if @array_p is sorted.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] cmp_fp  - comparator function pointer.
 *
 * @return: true if @array_p is sorted, false if @array_p is not sorted.
 */
bool darray_raw_is_sorted(const void* array_p, size_t size_of, size_t length, const compare_fp cmp_fp);


/*
 * Function check if @array_p is reverse sorted.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] cmp_fp  - comparator function pointer.
 *
 * @return: true if @array_p is sorted, false if @array_p is not sorted.
 */
bool darray_raw_is_reverse_sorted(const void* array_p, size_t size_of, size_t length, const compare_fp cmp_fp);


#endif /* DARRAY_RAW_H */
