#include <darray_raw/darray_raw.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


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


static bool mystruct_compare(const MyStructS* first_p, const MyStructS* second_p)
{
    if (first_p == NULL || second_p == NULL)
    {
        return false;
    }

    if (first_p->a == second_p->a && first_p->b == second_p->b && first_p->c == second_p->c)
    {
        return true;
    }

    return false;
}


static void test_darray_raw_create(void)
{
    register const size_t size_of = sizeof(int);
    register const size_t length = 10;

    int* array_p = darray_raw_create(size_of, length);
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

    int* array_p = darray_raw_create(size_of, length);
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


static void test_darray_raw_copy(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        array_p[i] = (int)(i + 1);
    }

    int* array_copy_p = darray_raw_create(size_of, length);
    assert(array_copy_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_copy_p[i] == 0);
    }

    register const int ret = darray_raw_copy(array_copy_p, array_p, size_of, length);
    assert(ret == 0);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_copy_p[i] == array_p[i]);
    }

    darray_raw_destroy(array_p);
    darray_raw_destroy(array_copy_p);
}


static void test_darray_raw_clone(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        array_p[i] = (int)(i + 1);
    }

    int* cloned_p = darray_raw_clone(array_p, size_of, length);
    assert(cloned_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == cloned_p[i]);
    }

    darray_raw_destroy(array_p);
    darray_raw_destroy(cloned_p);
}


static void test_darray_raw_move(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* first_array_p = darray_raw_create(size_of, length);
    assert(first_array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        first_array_p[i] = (int)(i + 1);
    }

    int* second_array_p = darray_raw_create(size_of, length);
    assert(second_array_p != NULL);

    /* check by copying memory */
    register const int ret1 = darray_raw_move(second_array_p, first_array_p, size_of, length); 
    assert(ret1 == 0);

    for (size_t i = 0; i < length; ++i)
    {
        assert(second_array_p[i] == first_array_p[i]);
    }

    /* check by moving memory */
    register const int ret2 = darray_raw_move(&first_array_p[length / 2], &first_array_p[0], size_of, length / 2);
    assert(ret2 == 0);

    for (size_t i = 0; i < length; ++i)
    {
        assert(first_array_p[i] == ((int[]){1, 2, 3, 4, 5, 1, 2, 3, 4, 5,})[i]);
    }

    darray_raw_destroy(first_array_p);
    darray_raw_destroy(second_array_p);
}


static void test_darray_raw_zeros(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        array_p[i] = (int)(i + 1);
    }

    register const int ret = darray_raw_zeros(array_p, size_of, length);
    assert(ret == 0);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == 0);
    }

    darray_raw_destroy(array_p);
}


static void test_darray_raw_set_all(void)
{
    register const size_t size_of = sizeof(MyStructS);
    register const int length = 10;

    MyStructS* mystruct_p = darray_raw_create(size_of, length);
    assert(mystruct_p != NULL);

    const MyStructS val = {2, 3, 5};

    register const int ret = darray_raw_set_all(mystruct_p, size_of, length, &val);
    assert(ret == 0);

    for (size_t i = 0; i < length; ++i)
    {
        assert(mystruct_compare(&mystruct_p[i], &val) == true);
    }

    darray_raw_destroy(mystruct_p);
}


int main(void)
{
    test_darray_raw_create();
    test_darray_raw_destroy();
    test_darray_raw_destroy_with_entires();
    test_darray_raw_copy();
    test_darray_raw_clone();
    test_darray_raw_move();
    test_darray_raw_zeros();
    test_darray_raw_set_all();

    return EXIT_SUCCESS;
}
