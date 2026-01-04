#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

void test_allocate_basic(void)
{
    obj *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    deallocate(allocation);
    shutdown();
}

void test_allocate_write_read(void)
{
    int *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    *allocation = 100;
    CU_ASSERT_EQUAL(*allocation, 100);
    deallocate(allocation);
    shutdown();
}

void test_allocate_null_destructor(void)
{
    obj *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    deallocate(allocation);
    shutdown();
}

void test_allocate_huge(void)
{
    size_t big_count = (((size_t) 1)<<38) - 1;
    
    obj **allocation = allocate(big_count, NULL);
    CU_ASSERT_PTR_NULL(allocation);
    
    shutdown();
}

void test_allocate_huge_huge(void)
{
    size_t big_count = (((size_t) 1)<<60) - 1;
    
    obj **allocation = allocate(big_count, NULL);
    CU_ASSERT_PTR_NULL(allocation);
    
    shutdown();
}

void register_allocate_tests(void)
{
    CU_pSuite suite = CU_add_suite("allocate()", 0, 0);

    CU_add_test(suite, "test allocate basic allocation", test_allocate_basic);
    CU_add_test(suite, "test allocate with write and read memory", test_allocate_write_read);
    CU_add_test(suite, "test allocate with null destructor", test_allocate_null_destructor);
    CU_add_test(suite, "test allocate with huge data", test_allocate_huge);
    CU_add_test(suite, "test allocate with huge data bigger than size", test_allocate_huge_huge);
}
