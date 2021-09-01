#include <darray_raw/darray_raw.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


typedef struct MyStructS
{
    size_t a;
    size_t b;
    size_t c;
} MyStructS;


static MyStructS* mystruct_create(const size_t a, const size_t b, const size_t c)
{
    MyStructS* mystruct_p = malloc(sizeof(*mystruct_p));

    if (mystruct_p == NULL)
    {
        perror("mystruct: malloc error");
        return NULL;
    }

    *mystruct_p = (MyStructS){ .a = a, .b = b, .c = c };

    return mystruct_p;
}


static void mystruct_destroy(void* mystruct_p)
{
    if (mystruct_p == NULL)
    {
        return;
    }

    MyStructS* ms_p = *(MyStructS**)mystruct_p;
    free(ms_p);
}


static void test_darray_raw_create(void)
{
    register const size_t size_of = sizeof(int);
    register const size_t length = 10;

    register int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == 0);
    }

    free(array_p);
}


static void test_darray_raw_destroy(void)
{
    register const size_t size_of = sizeof(int);
    register const size_t length = 10;

    register int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    darray_raw_destroy(array_p);
}


static void test_darray_raw_destroy_with_entires(void)
{
    register const size_t size_of = sizeof(MyStructS*);
    register const size_t length = 10;

    MyStructS** mystruct_pp = (MyStructS**)darray_raw_create(size_of, length);
    assert(mystruct_pp != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        mystruct_pp[i] = mystruct_create(i + 1, i + 2, i + 3);
        assert(mystruct_pp[i] != NULL);
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);
}


int main(void)
{
    test_darray_raw_create();
    test_darray_raw_destroy();
    test_darray_raw_destroy_with_entires();

    return 0;
}
