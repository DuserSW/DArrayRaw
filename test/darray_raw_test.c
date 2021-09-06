#include <darray_raw/darray_raw.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


typedef struct MyStructS
{
    size_t key;

    struct 
    {
        size_t a;   
        size_t b;
        size_t c;
    };
} MyStructS;


static int int_compare(const void* first_p, const void* second_p)
{
    assert(first_p != NULL);
    assert(second_p != NULL);

    register const int* const first_int_p = first_p;
    register const int* const second_int_p = second_p;

    if (*first_int_p > *second_int_p)
    {
        return 1;
    }

    if (*first_int_p == *second_int_p)
    {
        return 0;
    }

    return -1;
}


static MyStructS* mystruct_create(const size_t key, const size_t a, const size_t b, const size_t c)
{
    MyStructS* mystruct_p = malloc(sizeof(*mystruct_p));
    assert(mystruct_p != NULL);

    *mystruct_p = (MyStructS){ .key = key, .a = a, .b = b, .c = c };

    return mystruct_p;
}


static void mystruct_destroy(void* mystruct_p)
{
    assert(mystruct_p != NULL);

    MyStructS* ms_p = *(MyStructS**)mystruct_p;
    free(ms_p);
}


static int mystruct_compare(const void* first_p, const void* second_p)
{
    assert(first_p != NULL);
    assert(second_p != NULL);

    register const MyStructS* const mystruct_first_p = first_p;
    register const MyStructS* const mystruct_second_p = second_p;

    if (mystruct_first_p->key > mystruct_second_p->key)
    {
        return 1;
    }

    if (mystruct_first_p->key == mystruct_second_p->key)
    {
        return 0;
    }

    return -1;
}


static int mystruct_pointers_compare(const void* first_p, const void* second_p)
{
    assert(first_p != NULL);
    assert(second_p != NULL);

    register const MyStructS* const mystruct_first_p = *(const MyStructS* const *)first_p;
    register const MyStructS* const mystruct_second_p = *(const MyStructS * const *)second_p;

    if (mystruct_first_p->key > mystruct_second_p->key)
    {
        return 1;
    }

    if (mystruct_first_p->key == mystruct_second_p->key)
    {
        return 0;
    }

    return -1;
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
        mystruct_pp[i] = mystruct_create(i, i + 1, i + 2, i + 3);
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

    register int ret = -1;
    
    ret = darray_raw_copy(array_copy_p, array_p, size_of, length);
    assert(ret == 0);

    ret = memcmp(&array_p[0], &array_copy_p[0], size_of * length);
    assert(ret == 0);

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

    register const int ret = memcmp(&array_p[0], &cloned_p[0], size_of * length);
    assert(ret == 0);

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
        assert(first_array_p[i] == ((int[]){1, 2, 3, 4, 5, 1, 2, 3, 4, 5})[i]);
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

    register int ret = -1;
    
    ret = darray_raw_zeros(array_p, size_of, length);
    assert(ret == 0);

    const int array_to_expect[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    ret = memcmp(&array_p[0], &array_to_expect[0], size_of * length);
    assert(ret == 0);

    darray_raw_destroy(array_p);
}


static void test_darray_raw_set_all(void)
{
    register const size_t size_of = sizeof(MyStructS);
    register const int length = 10;

    MyStructS* mystruct_p = darray_raw_create(size_of, length);
    assert(mystruct_p != NULL);

    const MyStructS val = {0, {2, 3, 5}};

    register const int ret = darray_raw_set_all(mystruct_p, size_of, length, &val);
    assert(ret == 0);

    for (size_t i = 0; i < length; ++i)
    {
        assert(mystruct_compare(&mystruct_p[i], &val) == 0);
    }

    darray_raw_destroy(mystruct_p);
}


static void test_darray_raw_unsorted_insert_first(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == 0);
    }

    const int array_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    const int array_to_expect[][10] = 
    {
        {10, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {9, 10, 0, 0, 0, 0, 0, 0, 0, 0},
        {8, 9, 10, 0, 0, 0, 0, 0, 0, 0},
        {7, 8, 9, 10, 0, 0, 0, 0, 0, 0},
        {6, 7, 8, 9, 10, 0, 0, 0, 0, 0},
        {5, 6, 7, 8, 9, 10, 0, 0, 0, 0},
        {4, 5, 6, 7, 8, 9, 10, 0, 0, 0},
        {3, 4, 5, 6, 7, 8, 9, 10, 0, 0},
        {2, 3, 4, 5, 6, 7, 8, 9, 10, 0},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    };

    for (size_t i = 0; i < length; ++i)
    {
        register int ret = -1;
        
        ret = darray_raw_unsorted_insert_first(array_p, size_of, i + 1, &array_to_insert[i]);
        assert(ret == 0);

        ret = memcmp(&array_p[0], &array_to_expect[i][0], size_of * length);
        assert(ret == 0);
    }

    darray_raw_destroy(array_p);
}


static void test_darray_raw_unsorted_insert_last(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == 0);
    }

    const int array_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    const int array_to_expect[][10] = 
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 10},
        {0, 0, 0, 0, 0, 0, 0, 0, 9, 10},
        {0, 0, 0, 0, 0, 0, 0, 8, 9, 10},
        {0, 0, 0, 0, 0, 0, 7, 8, 9, 10},
        {0, 0, 0, 0, 0, 6, 7, 8, 9, 10},
        {0, 0, 0, 0, 5, 6, 7, 8, 9, 10},
        {0, 0, 0, 4, 5, 6, 7, 8, 9, 10},
        {0, 0, 3, 4, 5, 6, 7, 8, 9, 10},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    };

    for (size_t i = 0; i < length; ++i)
    {
        register int ret = -1;
        
        ret = darray_raw_unsorted_insert_last(array_p, size_of, length - i, &array_to_insert[i]);
        assert(ret == 0);

        ret = memcmp(&array_p[0], &array_to_expect[i][0], size_of * length);
        assert(ret == 0);
    }

    darray_raw_destroy(array_p);
}


static void test_darray_raw_unsorted_insert_pos(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == 0);
    }

    // 11,
    // 11, 99,
    // 11, 22, 99,
    // 11, 22, 99, 100,
    // 11, 22, 55, 99, 100,
    // 11, 22, 33, 55, 99, 100
    // 11, 22, 33, 55, 77, 99, 100
    // 11, 22, 33, 44, 55, 77, 99, 100
    // 11, 22, 33, 44, 55, 77, 88, 99, 100
    // 11, 22, 33, 44, 55, 66, 77, 88, 99, 100
    const int array_to_insert[] = {11, 99, 22, 100, 55, 33, 77, 44, 88, 66};
    const size_t indexes_to_insert[] = {0, 1, 1, 3, 2, 2, 4, 3, 6, 5};

    const int array_to_expect[][10] = 
    {
        {11,  0,  0,   0,   0,   0,   0,   0,   0,   0},
        {11, 99,  0,   0,   0,   0,   0,   0,   0,   0},
        {11, 22, 99,   0,   0,   0,   0,   0,   0,   0},
        {11, 22, 99, 100,   0,   0,   0,   0,   0,   0},
        {11, 22, 55,  99, 100,   0,   0,   0,   0,   0},
        {11, 22, 33,  55,  99, 100,   0,   0,   0,   0},
        {11, 22, 33,  55,  77,  99, 100,   0,   0,   0},
        {11, 22, 33,  44,  55,  77,  99, 100,   0,   0},
        {11, 22, 33,  44,  55,  77,  88,  99, 100,   0},
        {11, 22, 33,  44,  55,  66,  77,  88,  99, 100},
    };

    for (size_t i = 0; i < length; ++i)
    {
        register int ret = -1;
        
        ret = darray_raw_unsorted_insert_pos(array_p, size_of, length, indexes_to_insert[i], &array_to_insert[i]);
        assert(ret == 0);

        ret = memcmp(&array_p[0], &array_to_expect[i][0], size_of * length);
        assert(ret == 0);
    }

    darray_raw_destroy(array_p);
}


static void test_darray_raw_sorted_insert(void)
{
    register const size_t size_of = sizeof(int);
    register const int length = 10;

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        assert(array_p[i] == 0);
    }

    const int array_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    for (size_t i = 0; i < length; ++i)
    {
        register const int ret = darray_raw_sorted_insert(array_p, size_of, i + 1, &array_to_insert[i], int_compare);
        assert(ret == 0);
    }

    const int array_to_expect[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    register const int ret = memcmp(array_p, &array_to_expect[0], size_of * length);
    assert(ret == 0);

    darray_raw_destroy(array_p);
}


static void test_darray_raw_unsorted_insert_first_with_entry(void)
{
    register const size_t size_of = sizeof(MyStructS*);
    register const int length = 10;

    MyStructS** mystruct_pp = darray_raw_create(size_of, length);
    assert(mystruct_pp != NULL);

    const size_t array_key_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    const size_t array_key_to_expect[][10] = 
    {
        {10, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {9, 10, 0, 0, 0, 0, 0, 0, 0, 0},
        {8, 9, 10, 0, 0, 0, 0, 0, 0, 0},
        {7, 8, 9, 10, 0, 0, 0, 0, 0, 0},
        {6, 7, 8, 9, 10, 0, 0, 0, 0, 0},
        {5, 6, 7, 8, 9, 10, 0, 0, 0, 0},
        {4, 5, 6, 7, 8, 9, 10, 0, 0, 0},
        {3, 4, 5, 6, 7, 8, 9, 10, 0, 0},
        {2, 3, 4, 5, 6, 7, 8, 9, 10, 0},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    };

    for (size_t i = 0; i < length; ++i)
    {
        const MyStructS* const tmp_p = mystruct_create(array_key_to_insert[i], i + 1, i + 2, i + 3);
        assert(tmp_p != NULL);

        register const int ret = darray_raw_unsorted_insert_first(mystruct_pp, size_of, length, &tmp_p);       
        assert(ret == 0); 

        for (size_t j = 0; j < i + 1; ++j)
        {
            assert(mystruct_pp[j]->key == array_key_to_expect[i][j]);
        }
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);
}


static void test_darray_raw_unsorted_insert_last_with_entry(void)
{
    register const size_t size_of = sizeof(MyStructS*);
    register const int length = 10;

    MyStructS** mystruct_pp = darray_raw_create(size_of, length);
    assert(mystruct_pp != NULL);

    const size_t array_key_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    const size_t array_key_to_expect[][10] = 
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 10},
        {0, 0, 0, 0, 0, 0, 0, 0, 9, 10},
        {0, 0, 0, 0, 0, 0, 0, 8, 9, 10},
        {0, 0, 0, 0, 0, 0, 7, 8, 9, 10},
        {0, 0, 0, 0, 0, 6, 7, 8, 9, 10},
        {0, 0, 0, 0, 5, 6, 7, 8, 9, 10},
        {0, 0, 0, 4, 5, 6, 7, 8, 9, 10},
        {0, 0, 3, 4, 5, 6, 7, 8, 9, 10},
        {0, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
    };

    for (size_t i = 0; i < length; ++i)
    {        
        const MyStructS* const tmp_p = mystruct_create(array_key_to_insert[i], i + 1, i + 2, i + 3);
        assert(tmp_p != NULL);

        register const int ret = darray_raw_unsorted_insert_last(mystruct_pp, size_of, length - i, &tmp_p);
        assert(ret == 0);

        for (ssize_t j = (ssize_t)(length - 1); j > (ssize_t)(length - i - 2) && j >= 0; --j)
        {
            assert(mystruct_pp[j]->key == array_key_to_expect[i][j]);
        }
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);
}


static void test_darray_raw_unsorted_insert_pos_with_entry(void)
{
    register const size_t size_of = sizeof(MyStructS*);
    register const int length = 10;

    MyStructS** mystruct_pp = darray_raw_create(size_of, length);
    assert(mystruct_pp != NULL);

    // 11,
    // 11, 99,
    // 11, 22, 99,
    // 11, 22, 99, 100,
    // 11, 22, 55, 99, 100,
    // 11, 22, 33, 55, 99, 100
    // 11, 22, 33, 55, 77, 99, 100
    // 11, 22, 33, 44, 55, 77, 99, 100
    // 11, 22, 33, 44, 55, 77, 88, 99, 100
    // 11, 22, 33, 44, 55, 66, 77, 88, 99, 100
    const size_t array_key_to_insert[] = {11, 99, 22, 100, 55, 33, 77, 44, 88, 66};
    const size_t indexes_key_to_insert[] = {0, 1, 1, 3, 2, 2, 4, 3, 6, 5};

    const size_t array_key_to_expect[][10] = 
    {
        {11,  0,  0,   0,   0,   0,   0,   0,   0,   0},
        {11, 99,  0,   0,   0,   0,   0,   0,   0,   0},
        {11, 22, 99,   0,   0,   0,   0,   0,   0,   0},
        {11, 22, 99, 100,   0,   0,   0,   0,   0,   0},
        {11, 22, 55,  99, 100,   0,   0,   0,   0,   0},
        {11, 22, 33,  55,  99, 100,   0,   0,   0,   0},
        {11, 22, 33,  55,  77,  99, 100,   0,   0,   0},
        {11, 22, 33,  44,  55,  77,  99, 100,   0,   0},
        {11, 22, 33,  44,  55,  77,  88,  99, 100,   0},
        {11, 22, 33,  44,  55,  66,  77,  88,  99, 100},
    };

    for (size_t i = 0; i < length; ++i)
    {        
        const MyStructS* const tmp_p = mystruct_create(array_key_to_insert[i], i + 1, i + 2, i + 3);
        assert(tmp_p != NULL);

        register const int ret = darray_raw_unsorted_insert_pos(mystruct_pp, size_of, length, indexes_key_to_insert[i], &tmp_p);
        assert(ret == 0);

        for (size_t j = 0; j < i; ++j)
        {
            assert(mystruct_pp[j]->key == array_key_to_expect[i][j]);
        }
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);
}


static void test_darray_raw_sorted_insert_with_entry(void)
{
    register const size_t size_of = sizeof(MyStructS*);
    register const size_t length = 10;

    MyStructS** mystruct_pp = NULL; 
    MyStructS* tmp_p = NULL;

    /* ascending case */
    mystruct_pp = darray_raw_create(size_of, length);

    for (size_t i = 0; i < length; ++i)
    {
        register const size_t array_key_to_insert[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        tmp_p = mystruct_create(array_key_to_insert[i], i + 1, i + 2, i + 3);
        assert(tmp_p != NULL);

        register const int ret = darray_raw_sorted_insert(mystruct_pp, size_of, i + 1, &tmp_p, mystruct_pointers_compare);
        assert(ret == 0);
    }


    for (size_t i = 0; i < length; ++i)
    {
        register const size_t array_key_to_expect[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        assert(mystruct_pp[i]->key == array_key_to_expect[i]);
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);


    /* descending case */
    mystruct_pp = darray_raw_create(size_of, length);

    for (size_t i = 0; i < length; ++i)
    {
        register const size_t array_key_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

        tmp_p = mystruct_create(array_key_to_insert[i], i + 1, i + 2, i + 3);
        assert(tmp_p != NULL);

        register const int ret = darray_raw_sorted_insert(mystruct_pp, size_of, i + 1, &tmp_p, mystruct_pointers_compare);
        assert(ret == 0);
    }


    for (size_t i = 0; i < length; ++i)
    {
        register const size_t array_key_to_expect[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        assert(mystruct_pp[i]->key == array_key_to_expect[i]);
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);


    /* randomizing case */
    mystruct_pp = darray_raw_create(size_of, length);

    size_t array_key_to_insert[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    darray_raw_shuffle(&array_key_to_insert[0], size_of, length);

    for (size_t i = 0; i < length; ++i)
    {
        tmp_p = mystruct_create(array_key_to_insert[i], i + 1, i + 2, i + 3);
        assert(tmp_p != NULL);

        register const int ret = darray_raw_sorted_insert(mystruct_pp, size_of, i + 1, &tmp_p, mystruct_pointers_compare);
        assert(ret == 0);
    }


    for (size_t i = 0; i < length; ++i)
    {
        register const size_t array_key_to_expect[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        assert(mystruct_pp[i]->key == array_key_to_expect[i]);
    }

    darray_raw_destroy_with_entires(mystruct_pp, size_of, length, mystruct_destroy);
}


static void test_darray_raw_upper_bound(void)
{
    const int array[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144};

    register ssize_t upper_bound_index = -1;
    int search_key = 0;

    search_key = -1;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 0);

    search_key = 0;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 1);

    search_key = 1;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 3);

    search_key = 4;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 5);

    search_key = 13;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 8);

    search_key = 52;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 10);

    search_key = 79;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 11);

    search_key = 144;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 13);

    search_key = 223;
    upper_bound_index = darray_raw_upper_bound(&array[0], sizeof(*array), array_size(array), &search_key, int_compare);
    assert(upper_bound_index == 13);
}


static void test_darray_raw_shuffle(void)
{
    register const size_t size_of = sizeof(int);
    enum { length = 10 };

    int* array_p = darray_raw_create(size_of, length);
    assert(array_p != NULL);

    for (size_t i = 0; i < length; ++i)
    {
        array_p[i] = (int)(i + 1);
    }

    darray_raw_shuffle(array_p, size_of, length);
    
    register const bool ret = darray_raw_is_sorted(array_p, size_of, length, int_compare);
    assert(ret == false);

    /* @array_p have uniq values and @histogram check if shuffle did not overwrite some values. */
    int histogram[length];
    darray_raw_zeros(&histogram[0], size_of, length);

    for (size_t i = 0; i < length; ++i)
    {
        histogram[array_p[i] - 1]++;
    }

    for (size_t i = 0; i < length; ++i)
    {
        assert(histogram[i] == 1);
    }

    darray_raw_destroy(array_p);
}


static void test_darray_raw_is_sorted(void)
{
    register bool ret = false;

    const int sorted_array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ret = darray_raw_is_sorted(&sorted_array[0], sizeof(*sorted_array), array_size(sorted_array), int_compare);
    assert(ret == true);

    const int unsorted_array[] = {1, 2, 3, 44, 5, 6, 7, 8, 9, 10};
    ret = darray_raw_is_sorted(&unsorted_array[0], sizeof(*unsorted_array), array_size(unsorted_array), int_compare);
    assert(ret == false);
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
    test_darray_raw_unsorted_insert_first();
    test_darray_raw_unsorted_insert_last();
    test_darray_raw_unsorted_insert_pos();
    test_darray_raw_sorted_insert();
    test_darray_raw_unsorted_insert_first_with_entry();
    test_darray_raw_unsorted_insert_last_with_entry();
    test_darray_raw_unsorted_insert_pos_with_entry();
    test_darray_raw_sorted_insert_with_entry();
    test_darray_raw_upper_bound();
    test_darray_raw_shuffle();
    test_darray_raw_is_sorted();

    return EXIT_SUCCESS;
}
