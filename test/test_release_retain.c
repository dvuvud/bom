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

    shutdown();
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

    shutdown();
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

    shutdown();
}

void test_retain_release_null()
{
    retain(NULL);
    release(NULL);
}

// test release on zero refcount
void test_release_zero()
{
    set_cascade_limit(0);
    
    obj *allocation = allocate(sizeof(int), NULL);
    
    retain(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 1);

    release(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 0);

    release(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 0);
    
    set_cascade_limit(100);

    shutdown();
}

// more retain() than possible should do nothing
void test_retain_past_limit()
{
    obj *allocation = allocate(sizeof(int), NULL);
    
    // increment to max
    int REFCOUNT_MAX = 255;
    for (int i = 0; i < REFCOUNT_MAX; i++)
    {
        CU_ASSERT_EQUAL(rc(allocation), i);
        retain(allocation);
    }
    
    // keep at max
    CU_ASSERT_EQUAL(rc(allocation), REFCOUNT_MAX);
    retain(allocation);
    CU_ASSERT_EQUAL(rc(allocation), REFCOUNT_MAX);
    retain(allocation);
    retain(allocation);
    retain(allocation);
    retain(allocation);
    retain(allocation);
    CU_ASSERT_EQUAL(rc(allocation), REFCOUNT_MAX);
    
    // decrease back to 0
    for (int i = REFCOUNT_MAX; i > 0; i--)
    {
        CU_ASSERT_EQUAL(rc(allocation), i);
        release(allocation);
    }
    
    shutdown();
}

void register_retain_release_tests()
{
    CU_pSuite suite = CU_add_suite("retain & release", 0, 0);
    CU_add_test(suite, "test retain", test_retain);
    CU_add_test(suite, "test release", test_release);
    CU_add_test(suite, "test retain and release", test_retain_release);
    CU_add_test(suite, "test retain past limit", test_retain_past_limit);
    CU_add_test(suite, "test retain and release on null", test_retain_release_null);
    CU_add_test(suite, "test release on zero refcount", test_release_zero);
}
