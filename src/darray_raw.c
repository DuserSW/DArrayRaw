#include <darray_raw/darray_raw.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


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

    register uint8_t* restrict barray_p = array_p;

    for (size_t offset = 0; offset < size_of * length; offset += size_of)
    {
        destroy_fp(&barray_p[offset]);
    }

    free(array_p);
}
