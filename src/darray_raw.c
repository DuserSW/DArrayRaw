#include <darray_raw/darray_raw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


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
