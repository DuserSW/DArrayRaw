#include <darray_raw/darray_raw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Internal function which insert @data_p at @pos of @array_p.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] pos     - array index to insert data.
 * @param[in] data_p  - constant data which fill array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
static inline int __darray_raw_insert_pos(void* restrict array_p, size_t size_of, size_t length, size_t pos, const void* restrict data_p);


/*
 * Internal function which delete item at @pos of @array_p.
 * 
 * @param[in] array_p - pointer to array.
 * @param[in] size_of - size of each array member.
 * @param[in] length  - number of elements in array.
 * @param[in] pos     - array index to insert data.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
static inline int __darray_raw_delete_pos(void* array_p, size_t size_of, size_t length, size_t pos);


/*
 * Internal function which delete item at @pos of @array_p and call destructor on item.
 * 
 * @param[in] array_p    - pointer to array.
 * @param[in] size_of    - size of each array member.
 * @param[in] length     - number of elements in array.
 * @param[in] pos        - array index to insert data.
 * @param[in] destroy_fp - constant data which fill array.
 * 
 * @return: 0 on success, non-zero value on failure.
 */
static inline int __darray_raw_delete_pos_with_entry(void* array_p, size_t size_of, size_t length, size_t pos, const destructor_fp destroy_fp);


static inline int __darray_raw_insert_pos(void* const restrict array_p, const size_t size_of, const size_t length, const size_t pos, const void* const restrict data_p)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    if (pos >= length)
    {
        perror("DArrayRaw: argument pos is greater than length value\n");
        return -1;
    }

    if (data_p == NULL)
    {
        perror("DArrayRaw: argument data_p is NULL\n");
        return -1;
    }

    register uint8_t* const restrict barray_p = array_p;

    register void* dst_p = &barray_p[(pos + 1) * size_of];
    register void* src_p = &barray_p[pos * size_of];
    register const size_t bytes_to_move = (length - pos - 1) * size_of;

    if (memmove(dst_p, src_p, bytes_to_move) != dst_p)
    {
        perror("DArrayRaw: memmove error\n");
        return -1;
    }

    if (memcpy(src_p, data_p, size_of) != src_p)
    {
        perror("DArrayRaw: memcpy error\n");
        return -1;
    }

    return 0;
}


static inline int __darray_raw_delete_pos(void* array_p, size_t size_of, size_t length, size_t pos)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    if (pos >= length)
    {
        perror("DArrayRaw: argument pos is greater than length value\n");
        return -1;
    }

    register uint8_t* const restrict barray_p = array_p;

    register void* dst_p = &barray_p[pos * size_of];
    register const void* src_p = &barray_p[(pos + 1) * size_of];
    register const size_t bytes_to_move = (length - pos - 1) * size_of;

    if (bytes_to_move > 0)
    {
        if (memmove(dst_p, src_p, bytes_to_move) != dst_p)
        {
            perror("DArrayRaw: memmove error\n");
            return -1;
        }
    }

    if (memset(&barray_p[(length - 1) * size_of], 0, size_of) == NULL)
    {
        perror("DArrayRaw: memset error\n");
        return -1;
    }

    return 0;
}


static inline int __darray_raw_delete_pos_with_entry(void* array_p, size_t size_of, size_t length, size_t pos, const destructor_fp destroy_fp)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    if (pos >= length)
    {
        perror("DArrayRaw: argument pos is greater than length value\n");
        return -1;
    }

    if (destroy_fp == NULL)
    {
        perror("DArrayRaw: argument destroy_fp is NULL\n");
        return -1;
    }

    register uint8_t* const barray_p = array_p;

    register void* src_p = &barray_p[(pos + 1) * size_of];
    register void* dst_p = &barray_p[pos * size_of];
    register const size_t bytes_to_move = (length - pos - 1) * size_of;

    destroy_fp((void*)&barray_p[pos * size_of]);

    if (bytes_to_move > 0)
    {
        if (memmove(dst_p, src_p, bytes_to_move) != dst_p)
        {
            perror("DArrayRaw: memmove error\n");
            return -1;
        }
    }

    if (memset(&barray_p[(length - 1) * size_of], 0, size_of) == NULL)
    {
        perror("DArrayRaw: memset error\n");
        return -1;
    }

    return 0;
}


void* darray_raw_create(size_t size_of, size_t length)
{
    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return NULL;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return NULL;
    }

    void* array_p = calloc(length, size_of);

    if (array_p == NULL)
    {
        perror("DArrayRaw: calloc error\n");
        return NULL;
    }

    return array_p;
}


void darray_raw_destroy(void* array_p)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return;
    }

    free(array_p);
}


void darray_raw_destroy_with_entires(void* array_p, const size_t size_of, const size_t length, const destructor_fp destroy_fp)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return;
    }

    if (destroy_fp == NULL)
    {
        perror("DArrayRaw: argument destroy_fp is NULL\n");
    }

    register uint8_t* const restrict barray_p = array_p;

    for (size_t offset = 0; offset < size_of * length; offset += size_of)
    {
        destroy_fp(&barray_p[offset]);
    }

    free(array_p);
}


int darray_raw_copy(void* const restrict dst_array_p, void* const restrict src_array_p, const size_t size_of, const size_t length)
{
    if (dst_array_p == NULL)
    {
        perror("DArrayRaw: argument dst_array_p is NULL\n");
        return -1;
    }

    if (src_array_p == NULL)
    {
        perror("DArrayRaw: argument src_array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    return (memcpy(dst_array_p, src_array_p, size_of * length) == dst_array_p) ? 0 : -1;
}


void* darray_raw_clone(void* const array_p, const size_t size_of, const size_t length)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return NULL;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return NULL;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return NULL;
    }

    register void* const restrict clone_array_p = darray_raw_create(size_of, length);

    if (clone_array_p == NULL)
    {
        perror("DArrayRaw: darray_raw_create error\n");
    }

    register const int ret = darray_raw_copy(clone_array_p, array_p, size_of, length);

    if (ret == -1)
    {
        perror("DArrayRaw: darray_raw_copy error\n");
        darray_raw_destroy(clone_array_p);

        return NULL;
    }

    return clone_array_p;
}


int darray_raw_move(void* const dst_array_p, void* const src_array_p, const size_t size_of, const size_t length)
{
    if (dst_array_p == NULL)
    {
        perror("DArrayRaw: argument dst_array_p is NULL\n");
        return -1;
    }

    if (src_array_p == NULL)
    {
        perror("DArrayRaw: argument src_array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    return (memmove(dst_array_p, src_array_p, size_of * length) == dst_array_p) ? 0 : -1;
}


int darray_raw_zeros(void* const array_p, const size_t size_of, const size_t length)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    return (memset(array_p, 0, size_of *length) == array_p) ? 0 : -1;
}


int darray_raw_set_all(void* const restrict array_p, const size_t size_of, const size_t length, const void* const restrict data_p)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    if (data_p == NULL)
    {
        perror("DArrayRaw: argument data_p is NULL\n");
        return -1;
    }

    register uint8_t* const restrict barray_p = array_p;

    for (size_t offset = 0; offset < size_of * length; offset += size_of)
    {
        memcpy(&barray_p[offset], data_p, size_of);
    }

    return 0;
}


int darray_raw_unsorted_insert_first(void* const restrict array_p, const size_t size_of, const size_t length, const void* const restrict data_p)
{
    return __darray_raw_insert_pos(array_p, size_of, length, 0, data_p);
}


int darray_raw_unsorted_insert_last(void* const restrict array_p, const size_t size_of, const size_t length, const void* const restrict data_p)
{
    return __darray_raw_insert_pos(array_p, size_of, length, length - 1, data_p);
}


int darray_raw_unsorted_insert_pos(void* const restrict array_p, const size_t size_of, const size_t length, const size_t pos, const void* const restrict data_p)
{
    return __darray_raw_insert_pos(array_p, size_of, length, pos, data_p);
}


int darray_raw_sorted_insert(void* const restrict array_p, const size_t size_of, const size_t length, const void* const restrict data_p, const compare_fp cmp_fp)
{
    if (length == 1)
    {
        return __darray_raw_insert_pos(array_p, size_of, length, 0, data_p);
    }

    register const ssize_t upper_bound_index = darray_raw_upper_bound(array_p, size_of, length - 1, data_p, cmp_fp);

    if (upper_bound_index == -1)
    {
        perror("DArrayRaw: darray_raw_upper_bound error\n");
        return -1;
    }

    return __darray_raw_insert_pos(array_p, size_of, length, (size_t)upper_bound_index, data_p);
}


int darray_raw_delete_first(void* const array_p, const size_t size_of, const size_t length)
{
    return __darray_raw_delete_pos(array_p, size_of, length, 0);
}


int darray_raw_delete_last(void* const array_p, const size_t size_of, const size_t length)
{
    return __darray_raw_delete_pos(array_p, size_of, length, length - 1);
}


int darray_raw_delete_pos(void* const array_p, const size_t size_of, const size_t length, const size_t pos)
{
    return __darray_raw_delete_pos(array_p, size_of, length, pos);
}


int darray_raw_delete_all(void* const array_p, const size_t size_of, const size_t length)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    return darray_raw_zeros(array_p, size_of, length);
}


int darray_raw_delete_first_with_entry(void* const array_p, const size_t size_of, const size_t length, const destructor_fp destroy_fp)
{
    return __darray_raw_delete_pos_with_entry(array_p, size_of, length, 0, destroy_fp);
}


int darray_raw_delete_last_with_entry(void* const array_p, const size_t size_of, const size_t length, const destructor_fp destroy_fp)
{
    return __darray_raw_delete_pos_with_entry(array_p, size_of, length, length - 1, destroy_fp);
}


int darray_raw_delete_pos_with_entry(void* const array_p, const size_t size_of, const size_t length, const size_t pos, const destructor_fp destroy_fp)
{
    return __darray_raw_delete_pos_with_entry(array_p, size_of, length, pos, destroy_fp);
}


int darray_raw_delete_all_with_entries(void* const array_p, const size_t size_of, const size_t length, const destructor_fp destroy_fp)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    if (destroy_fp == NULL)
    {
        perror("DArrayRaw: argument destroy_fp is NULL\n");
        return -1;
    }

    register uint8_t* const barray_p = array_p;

    for (size_t offset = 0; offset < size_of * length; offset += size_of)
    {
        destroy_fp((void*)&barray_p[offset]);
    }

    return darray_raw_zeros(array_p, size_of, length);
}


ssize_t darray_raw_upper_bound(const void* const restrict array_p, const size_t size_of, const size_t length, const void* const restrict data_p, const compare_fp cmp_fp)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return -1;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return -1;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return -1;
    }

    if (data_p == NULL)
    {
        perror("DArrayRaw: argument data_p is NULL\n");
        return -1;
    }

    if (cmp_fp == NULL)
    {
        perror("DArrayRaw: argument cmp_fp is NULL\n");
        return -1;
    }

    register const uint8_t* const restrict barray_p = array_p;

    register size_t offset_left = 0;
    register size_t offset_right = length * size_of;

    while (offset_left < offset_right)
    {
        register const size_t offset_middle = (((offset_left / size_of) + (offset_right / size_of)) / 2) * size_of;

        if (cmp_fp(data_p, &barray_p[offset_middle]) >= 0)
        {
            offset_left = offset_middle + size_of;
        }
        else
        {
            offset_right = offset_middle;
        }
    }

    return (ssize_t)(offset_left / size_of);
}


void darray_raw_shuffle(void* const array_p, const size_t size_of, const size_t length)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return;
    }

    register uint8_t* const barray = array_p;

    for (size_t i = length - 1; i > 0; --i)
    {
        register const size_t rand_nr = (size_t)rand() % (i + 1);

        if (i != rand_nr)
        {
            swap(barray[i * size_of], barray[rand_nr * size_of], size_of);
        }
    }
}


bool darray_raw_is_sorted(const void* const array_p, const size_t size_of, const size_t length, const compare_fp cmp_fp)
{
    if (array_p == NULL)
    {
        perror("DArrayRaw: argument array_p is NULL\n");
        return false;
    }

    if (size_of == 0)
    {
        perror("DArrayRaw: argument size_of has to small value\n");
        return false;
    }

    if (length == 0)
    {
        perror("DArrayRaw: argument length has to small value\n");
        return false;
    }

    if (cmp_fp == NULL)
    {
        perror("DArrayRaw: argument cmp_fp is NULL\n");
        return false;
    }

    const uint8_t* const barray_p = array_p;

    for (size_t offset = 0; offset < size_of * (length - 1); offset += size_of)
    {
        if (cmp_fp(&barray_p[offset], &barray_p[offset + size_of]) > 0)
        {
            return false;
        }
    }

    return true;
}
