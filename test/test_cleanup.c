#include <CUnit/Basic.h>
#include "../src/include/refmem.h"

// Test: cleanup on unretained objects.
void test_cleanup_unretained()
{
    obj *o = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(o);
    CU_ASSERT_EQUAL(rc(o), 0);

    cleanup();

    // cleanup shouldn't free o, so running deallocate shouldn't cause an error
    deallocate(o);

    shutdown();
}

// Test: objects with refcount > 0 should not be released with cleanup.
void test_cleanup_keeps_retained()
{
    obj *a = allocate(sizeof(int), NULL);
    obj *b = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(b);

    retain(a);
    retain(b);

    cleanup();
    CU_ASSERT_EQUAL(rc(a), 1);
    CU_ASSERT_EQUAL(rc(b), 1);

    release(a);
    release(b);

    shutdown();
}

// Test: ignores the cascade limit with cleanup.
void test_cleanup_cascade_limit()
{
    set_cascade_limit(0);

    obj *a = allocate(sizeof(int), NULL);
    obj *b = allocate(sizeof(int), NULL);
    obj *c = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(a);
    CU_ASSERT_PTR_NOT_NULL(b);
    CU_ASSERT_PTR_NOT_NULL(c);

    retain(a);
    retain(b);
    retain(c);
    CU_ASSERT_EQUAL(rc(a), 1);
    CU_ASSERT_EQUAL(rc(b), 1);
    CU_ASSERT_EQUAL(rc(c), 1);

    // cascade limit is 0 so none of these would be destroyed
    release(a);
    release(b);
    release(c);
    CU_ASSERT_EQUAL(rc(a), 0);
    CU_ASSERT_EQUAL(rc(b), 0);
    CU_ASSERT_EQUAL(rc(c), 0);

    // should ignore limit
    cleanup();

    set_cascade_limit(100); // set limit back to 100 between tests since its static memory

    shutdown();
}

static int destroyed = 0;

void test_destructor_cleanup(obj *o)
{
    destroyed++;
}

void test_cleanup_calls_destructor(void)
{
    set_cascade_limit(0);

    destroyed = 0;

    obj *o = allocate(sizeof(int), test_destructor_cleanup);
    CU_ASSERT_PTR_NOT_NULL(o);

    retain(o);
    CU_ASSERT_EQUAL(rc(o), 1);

    release(o);
    CU_ASSERT_EQUAL(rc(o), 0);

    cleanup();

    CU_ASSERT_EQUAL(destroyed, 1);

    set_cascade_limit(100); // set limit back to 100 between tests since its static memory

    shutdown();
}

void test_shutdown_calls_destructor(void)
{
    destroyed = 0;

    obj *o = allocate(sizeof(int), test_destructor_cleanup);
    retain(o);
    release(o);

    shutdown();

    CU_ASSERT_EQUAL(destroyed, 1);
}



void register_cleanup_shutdown_tests()
{
    CU_pSuite suite = CU_add_suite("cleanup_shutdown_tests", NULL, NULL);
    if (suite != NULL)
    {
        CU_add_test(suite, "test cleanup frees unretained objects", test_cleanup_unretained);
        CU_add_test(suite, "test only cleans refcont == 0", test_cleanup_keeps_retained);
        CU_add_test(suite, "test ignores cascade limit", test_cleanup_cascade_limit);
        CU_add_test(suite, "test cleanup with destructor", test_cleanup_calls_destructor);
        CU_add_test(suite, "test shutdown with destructor", test_shutdown_calls_destructor);
    }
}
