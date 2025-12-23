#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

void test_rc_init_zero()
{
    obj *allocation = allocate(sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);
    CU_ASSERT_EQUAL(rc(allocation), 0);
}

void test_rc_null()
{
    CU_ASSERT_EQUAL(rc(NULL), 0);
}

void register_rc_tests()
{
    CU_pSuite suite = CU_add_suite("rc()", 0, 0);
    CU_add_test(suite, "test rc initial is zero", test_rc_init_zero);
    CU_add_test(suite, "test rc null returns zero", test_rc_null);
}
