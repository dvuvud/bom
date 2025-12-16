#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

void test_allocate_array_basic()
{
	int *allocation = allocate_array(5, sizeof(int), NULL);
	CU_ASSERT_PTR_NOT_NULL(allocation);

    //FRIGÖRING
}

void test_allocate_array_zero()
{
    int *allocation = allocate_array(0, sizeof(int), NULL);
	CU_ASSERT_PTR_NULL(allocation);

    //FRIGÖRING
}

void test_allocate_array_zero_init()
{
    int *allocation = allocate_array(5, sizeof(int), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++){
        CU_ASSERT_EQUAL(allocation[i], 0);
    }

    //FRIGÖRING
}

void test_allocate_array_ptr(){
    char **allocation = allocate_array(5, sizeof(char *), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++) {
        CU_ASSERT_PTR_NULL(allocation[i]);
    }

    //FRIGÖRING
}

void test_allocate_array_ptr_obj(){
    obj **allocation = allocate_array(5, sizeof(obj *), NULL);
    CU_ASSERT_PTR_NOT_NULL(allocation);

    for (int i = 0; i < 5; i++) {
        CU_ASSERT_PTR_NULL(allocation[i]);
    }

    //FRIGÖRING
}



int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Allocate_array", 0, 0);
    CU_add_test(suite, "test allocate_array",                       test_allocate_array_basic);
    CU_add_test(suite, "test allocate_array with 0 elements",       test_allocate_array_zero);
    CU_add_test(suite, "test allocate_array with initialized to 0", test_allocate_array_zero_init);
    CU_add_test(suite, "test allocate_array with array of char *",  test_allocate_array_ptr);
    CU_add_test(suite, "test allocate_array with array of obj *",  test_allocate_array_ptr_obj);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}