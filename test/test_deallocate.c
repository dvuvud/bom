#include <CUnit/Basic.h>
#include "../src/include/refmem.h"

static int destructor_calls = 0;

//destructor for test(counts use)
void test_destructor(obj *p)
{
    (void)p;
    destructor_calls++;
}

void test_deallocate_null(void)
{
    deallocate(NULL);
    CU_ASSERT_TRUE(1); // nothing happens
}

void test_deallocate_calls_destructor(void)
{
    destructor_calls = 0;

    obj *p = allocate(sizeof(int), test_destructor);
    CU_ASSERT_PTR_NOT_NULL(p);
    CU_ASSERT_EQUAL(rc(p), 0);

    deallocate(p);

    CU_ASSERT_EQUAL(destructor_calls, 1);

    shutdown();
}

void test_deallocate_with_rc_zero(void)
{
    obj *p = allocate_array(1, sizeof(int), NULL);
    CU_ASSERT_EQUAL(rc(p), 0);

    deallocate(p);
    CU_ASSERT_TRUE(1);
    
    shutdown();
}

void test_deallocate_with_rc_not_zero(void)
{
    destructor_calls = 0;

    obj *p = allocate(sizeof(int), test_destructor);
    retain(p); // rc = 1

    deallocate(p);

    CU_ASSERT_EQUAL(destructor_calls, 0);

    release(p);

    shutdown();
}

// test release and retain on address not in hashset
void test_deallocate_on_address_not_allocated(void)
{
    obj *p = allocate(1, NULL);
    deallocate((obj *)0x1234);
    CU_ASSERT_PTR_NOT_NULL(p);
    deallocate(p);
    shutdown();
}

void register_deallocate_tests(void)
{
    CU_pSuite suite = CU_add_suite("Deallocate", NULL, NULL);
    CU_add_test(suite, "test if deallocate input NULL is safe", test_deallocate_null);
    CU_add_test(suite, "test if deallocate calls destructor", test_deallocate_calls_destructor);
    CU_add_test(suite, "test if deallocate allows rc == 0", test_deallocate_with_rc_zero);
    CU_add_test(suite, "test if deallocate handles rc != 0", test_deallocate_with_rc_not_zero);
    CU_add_test(suite, "test deallocate on non-allocated address", test_deallocate_on_address_not_allocated);
}
