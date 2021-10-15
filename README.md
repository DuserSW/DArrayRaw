## DArrayRaw (Duser Array Raw)
DArrayRaw is standalone library that allows you to avoid many low-level operation on raw arrays. Framework is very useful because might be used as low-level layer that will be used by high level applications. Library is lightweight and written with a view to best performance. If there is lack of informations or features that you need, please feel free to contant with me. I will try to help!

## Platform
- Supported only Linux platform. Tested on Ubuntu 20.04 LTS.

## Requirements
- Makefile.
- Compiler: clang or gcc. At least standard C17 and GNU dialect.
- Valgrind (for unit tests verification).

## How to build
There is six available options in Makefile:
````
all - build DArrayRaw library with unit tests as examples.
lib - build only DArrayRaw library.
test - build only DArrayRaw unit tests.
install - build DArrayRaw library and copy necessary files for specified directory.
clean - remove all files related with compilation process.
memcheck - this option will run valgrind to verify possibility of memory leaks.
help - this option will print all available option in Makefile.
````

## How to import
Let's assume that your project where you want to use DArrayRaw has following structure. External directory is a place where you keep libraries needed by your application.
````
├── external
├── inc
├── src
├── tests
├── app
````

There is two available options to install library. Both do the same!
````
by using: make install
by using script: install_darray_raw.sh
````

#### Then you need to do following steps:
````
1. Download DArrayRaw repository and go into them.
2. Install library in one way:
    * $make install P=/home/$user/project/external
    * $scripts/install_darray_raw.sh /home/$user/project/external
3. You need to link two things: library and header files. Add below options to compilation process.
    * -I/home/$user/project/external/darray_raw/inc -L/home/$user/project/external/darray_raw -ldarray_raw
4. If you need better backtrace in logs or for debugging, you can pass flag -rdynamic. 
   (Please remember that binary size will increase)
5. In the source file where you want to use DArrayRaw include main header: #include <darray_raw/darray_raw.h>
6. Now you can use DArrayRaw library. Please read features, functions documentation and understand examples.
````

## Features
- create raw array (with initialization) and destroy (with/without entries).
- copy/clone/move/zeros/set_all for raw arrays.
- insert as first/last/position with/without entries for unsorted raw arrays.
- insert for sorted raw arrays.
- delete first/last/position/all with/without entires for raw arrays.
- find lower/upper bound for sorted raw arrays.
- find minimum/maximum value for raw arrays.
- find first/last value for sorted/unsorted raw arrays.
- sort/shuffle/reverse raw arrays.
- check is two raw arrays are equals.
- check if raw array is sorted/reverse sorted.

## Documentation
For examples of usage in code please take a look for unit tests. Each of function has at least one unit test. Below header shows power of this small library:
```
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
 * Function allocate memory for array on heap. By default array is zeros.
 *
 * @param[in] type - type of each of array member.
 * @param[in] ...  - variadic arguments which initialize array.
 * 
 * @return: allocated and initialized array on success, NULL on failure.
 */
#define darray_raw_create_and_init(type, ...) \
    darray_raw_create_and_init(sizeof(type), VA_ARGS_LENGTH(__VA_ARGS__), (type[VA_ARGS_LENGTH(__VA_ARGS__)]){__VA_ARGS__})

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
int darray_raw_copy(void* restrict dst_array_p, const void* restrict src_array_p, size_t size_of, size_t length);

/*
 * Function clone array @array_p based on size calculated from @size_of multiply @length.
 *
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * 
 * @return: cloned array on success, NULL on failure.
 */
void* darray_raw_clone(const void* array_p, size_t size_of, size_t length);

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
int darray_raw_move(void* dst_array_p, const void* src_array_p, size_t size_of, size_t length);

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
 * Function get lower bound of @data_p from @array_p.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] data_p  - constant data which fill array.
 * @param[in] cmp_fp  - comparator function pointer.
 * 
 * @return: lower bound index on success, -1 value on failure.
 */
ssize_t darray_raw_lower_bound(const void* restrict array_p, size_t size_of, size_t length, const void* restrict data_p, const compare_fp cmp_fp);

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
 * Function find minimum value from @array_p.
 * Value under found index will be copy into @out_p if not NULL.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * @param[out] out_p   - copy found value if not NULL.
 * 
 * @return: minimum value index on success, -1 value on failure.
 */
ssize_t darray_raw_find_min(const void* array_p, size_t size_of, size_t length, const compare_fp cmp_fp, void* out_p);

/*
 * Function find maximum value from @array_p.
 * Value under found index will be copy into @out_p if not NULL.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * @param[out] out_p   - copy found value if not NULL.
 * 
 * @return: maximum value index on success, -1 value on failure.
 */
ssize_t darray_raw_find_max(const void* array_p, size_t size_of, size_t length, const compare_fp cmp_fp, void* out_p);

/*
 * Function find first occurrence of @key_p in unsorted @array_p. 
 * Value under found index will be copy into @out_p if not NULL.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  key_p   - search first key from array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * @param[out] out_p   - copy found value if not NULL.
 * 
 * @return: index of first occurrence on success, -1 value on failure.
 */
ssize_t darray_raw_unsorted_find_first(const void* restrict array_p, size_t size_of, size_t length, const void* restrict key_p, const compare_fp cmp_fp, void* out_p);

/*
 * Function last occurrence of @key_p in unsorted @array_p. 
 * Value under found index will be copy into @out_p if not NULL.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  key_p   - search last key from array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * @param[out] out_p   - copy found value if not NULL.
 * 
 * @return: index of last occurrence on success, -1 value on failure.
 */
ssize_t darray_raw_unsorted_find_last(const void* restrict array_p, size_t size_of, size_t length, const void* restrict key_p, const compare_fp cmp_fp, void* out_p);

/*
 * Function find first occurrence of @key_p in sorted @array_p. 
 * Value under found index will be copy into @out_p if not NULL.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  key_p   - search first key from array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * @param[out] out_p   - copy found value if not NULL.
 * 
 * @return: index of first occurrence on success, -1 value on failure.
 */
ssize_t darray_raw_sorted_find_first(const void* restrict array_p, size_t size_of, size_t length, const void* restrict key_p, const compare_fp cmp_fp, void* out_p);

/*
 * Function last occurrence of @key_p in sorted @array_p. 
 * Value under found index will be copy into @out_p if not NULL.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  key_p   - search last key from array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * @param[out] out_p   - copy found value if not NULL.
 * 
 * @return: index of last occurrence on success, -1 value on failure.
 */
ssize_t darray_raw_sorted_find_last(const void* restrict array_p, size_t size_of, size_t length, const void* restrict key_p, const compare_fp cmp_fp, void* out_p);

/*
 * Function sort @array_p. 
 * Insertion-sort will be used for arrays with length smaller than 17 elements. For bigger arrays dual-pivot quick-sort will be used.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * @param[in]  cmp_fp  - comparator function pointer.
 * 
 * @return: this is void function.
 */
void darray_raw_sort(void* array_p, size_t size_of, size_t length, const compare_fp cmp_fp);

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
 * Function reverse @array_p.
 * 
 * @param[in]  array_p - pointer to array.
 * @param[in]  size_of - size of each array member.
 * @param[in]  length  - number of elements in array.
 * 
 * @return: this is void function.
 */
void darray_raw_reverse(void* array_p, size_t size_of, size_t length);

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
 * @return: true if @array_p is reverse sorted, false if @array_p is not reverse sorted.
 */
bool darray_raw_is_reverse_sorted(const void* array_p, size_t size_of, size_t length, const compare_fp cmp_fp);
```

## Contact
Email: kamilkielbasa64@gmail.com