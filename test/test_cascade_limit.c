#include <CUnit/Basic.h>
#include "../src/include/refmem.h"

// Tests set & get.
void test_set_get_cascade_limit() 
{
	set_cascade_limit(50);
	CU_ASSERT_EQUAL(get_cascade_limit(), 50);

	set_cascade_limit(0);
	CU_ASSERT_EQUAL(get_cascade_limit(), 0);

	set_cascade_limit(100);
	CU_ASSERT_EQUAL(get_cascade_limit(), 100);
}

// Test to ensure large values are handled correctly.
void test_cascade_limit_large_value() 
{
	set_cascade_limit(1000);
	CU_ASSERT_EQUAL(get_cascade_limit(), 1000);
}


void register_cascade_limit_tests() 
{
	CU_pSuite suite = CU_add_suite("Cascade_Limit_Tests", NULL, NULL);
	if (suite != NULL) 
	{
		CU_add_test(suite, "test set and get cascade limit", test_set_get_cascade_limit);
		CU_add_test(suite, "test large cascade limit value", test_cascade_limit_large_value);
	}
}
