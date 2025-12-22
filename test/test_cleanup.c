#include <CUnit/Basic.h>
#include "../src/include/refmem.h"

// Test: cleanup frees unretained objects.
void test_cleanup_unretained()
{
    obj *o = allocate(sizeof(int), NULL);
    CU_ASSERT_NOT_NULL(0);
    CU_ASSERT_EQUAL(rc(o), 0);

    cleanup();

    /* cannot check for dangling pointers but valgrind should not
       show any leaks.*/
}

// Test: objects with refcount > 0 should not be released with cleanup.
void test_cleanup_keeps_retained()
{
    obj *a = allocate(sizeof(int), NULL);
    obj *b = allocate(sizeof(int), NULL);

    retain(a);

    cleanup();
    CU_ASSERT_EQUAL(rc(a), 1);

    release(a);

    /* valgrind should not show any leaks.*/
}

// Test: ignores the cascade limit with cleanup.
void test_cleanup_cascade_limit()
{
    set_cascade_limit(1);

    obj *a = allocate(sizeof(int), NULL);
    obj *b = allocate(sizeof(int), NULL);
    obj *c = allocate(sizeof(int), NULL);

    cleanup();

    /* all three objects should be removed, valgrind should not
       show any leaks.*/
}

static int destroyed = 0;

void test_destructor(obj *o)
{
    destroyed++;
}

// Test: cleanup runs the destructor as it should.
void test_cleanup_calls_destructor(void)
{
    destroyed = 0;

    obj *o = allocate(sizeof(int), test_destructor);
    cleanup();

    CU_ASSERT_EQUAL(destroyed, 1);
}

// Test: shutdown runs the destructor as it should.
void test_shutdown_calls_destructor(void)
{
    destroyed = 0;

    obj *o = allocate(sizeof(int), test_destructor);
    retain(o);

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