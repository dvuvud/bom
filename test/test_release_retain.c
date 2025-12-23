#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

void test_retain()
{
    obj *allocation = allocate(sizeof(int), NULL);

    retain(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 1);
    release(allocation);
}

void test_release()
{
    obj *allocation = allocate(sizeof(int), NULL);

    retain(allocation);
    retain(allocation);
    retain(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 3);

    release(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 2);

    release(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 1);

    release(allocation);
}

void test_retain_release()
{
    set_cascade_limit(0);

    obj *allocation = allocate(sizeof(int), NULL);

    retain(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 1);

    release(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 0);

    cleanup();

    set_cascade_limit(100);
}

void register_retain_release_tests()
{
    CU_pSuite suite = CU_add_suite("retain & release", 0, 0);
    CU_add_test(suite, "test retain", test_retain);
    CU_add_test(suite, "test release", test_release);
    CU_add_test(suite, "test retain and release", test_retain_release);
}
