#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

void test_allocate_basic()
{
    obj *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    deallocate(allocation);
    shutdown();
}

void test_allocate_write_read()
{
    int *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    *allocation = 100;
    CU_ASSERT_EQUAL(*allocation, 100);
    deallocate(allocation);
    shutdown();
}

void test_allocate_null_destructor()
{
    obj *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    deallocate(allocation);
    shutdown();
}

void register_allocate_tests()
{
    CU_pSuite suite = CU_add_suite("allocate()", 0, 0);

    CU_add_test(suite, "test allocate basic allocation", test_allocate_basic);
    CU_add_test(suite, "test allocate with write and read memory", test_allocate_write_read);
    CU_add_test(suite, "test allocate with null destructor", test_allocate_null_destructor);
}
