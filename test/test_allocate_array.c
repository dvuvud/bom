#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"


void test_allocate_array_basic()
{
    int *allocation = allocate_array(5, sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++){
        CU_ASSERT_EQUAL(arr[i], 0);
    }
}

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Allocate_array", 0, 0);
    CU_add_test(suite, "test allocate_array with refcount 0", test_allocate_array_basic);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}