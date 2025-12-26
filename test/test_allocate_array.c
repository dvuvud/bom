#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

void test_allocate_array_basic()
{
    int *allocation = allocate_array(5, sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    deallocate(allocation);

    shutdown();
}

void test_allocate_array_zero()
{
    int *allocation = allocate_array(0, sizeof(int), NULL);
    CU_ASSERT_PTR_NULL(allocation);

    deallocate(allocation);

    shutdown();
}

void test_allocate_array_zero_size()
{
    int *allocation = allocate_array(5, 0, NULL);
    CU_ASSERT_PTR_NULL(allocation);
    
    allocation = allocate_array(0, 0, NULL);
    CU_ASSERT_PTR_NULL(allocation);

    shutdown();
}

void test_allocate_array_zero_init()
{
    int *allocation = allocate_array(5, sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++){
        CU_ASSERT_EQUAL(allocation[i], 0);
    }

    deallocate(allocation);

    shutdown();
}

void test_allocate_array_ptr()
{
    char **allocation = allocate_array(5, sizeof(char *), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++) {
        CU_ASSERT_PTR_NULL(allocation[i]);
    }

    deallocate(allocation);

    shutdown();
}

void test_allocate_array_ptr_obj()
{
    obj **allocation = allocate_array(5, sizeof(obj *), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++) {
        CU_ASSERT_PTR_NULL(allocation[i]);
    }

    deallocate(allocation);

    shutdown();
}

void test_allocate_array_huge()
{
    size_t big_count = (((size_t) 1)<<38) - 1;
    int half_count = 1<<19;
    
    obj **allocation = allocate_array(big_count, sizeof(int), NULL);
    CU_ASSERT_PTR_NULL(allocation);
    
    allocation = allocate_array(2, big_count, NULL);
    CU_ASSERT_PTR_NULL(allocation);
    
    allocation = allocate_array(half_count, half_count, NULL);
    CU_ASSERT_PTR_NULL(allocation);
    
    shutdown();
}

void register_array_allocation_tests()
{
    CU_pSuite suite = CU_add_suite("Allocate_array", 0, 0);
    CU_add_test(suite, "test allocate_array", test_allocate_array_basic);
    CU_add_test(suite, "test allocate_array with 0 elements", test_allocate_array_zero);
    CU_add_test(suite, "test allocate_array with 0-sized elements", test_allocate_array_zero_size);
    CU_add_test(suite, "test allocate_array with initialized to 0", test_allocate_array_zero_init);
    CU_add_test(suite, "test allocate_array with array of char *", test_allocate_array_ptr);
    CU_add_test(suite, "test allocate_array with array of obj *", test_allocate_array_ptr_obj);
    CU_add_test(suite, "test allocate_array with huge data", test_allocate_array_huge);
}

