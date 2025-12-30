#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <refmem.h>

// standard string duplication
void test_refmem_strdup_basic(void)
{
    const char *original = "Hello CUnit";

    char *copy = refmem_strdup(original);

    CU_ASSERT_PTR_NOT_NULL(copy);
    CU_ASSERT_STRING_EQUAL(copy, original);
    CU_ASSERT_PTR_NOT_EQUAL(copy, original);

    deallocate(copy);
    shutdown();
}

// input is NULL
void test_refmem_strdup_null_input(void)
{
    char *copy = refmem_strdup(NULL);

    // Should return NULL immediately
    CU_ASSERT_PTR_NULL(copy);
}

// empty String
void test_refmem_strdup_empty_string(void)
{
    const char *original = "";

    char *copy = refmem_strdup(original);

    CU_ASSERT_PTR_NOT_NULL(copy);

    deallocate(copy);
    shutdown();
}

void register_strdup_tests(void) {
    CU_pSuite suite = CU_add_suite("strdup", NULL, NULL);

    CU_add_test(suite, "test basic string duplication", test_refmem_strdup_basic);
    CU_add_test(suite, "test strdup on null", test_refmem_strdup_null_input);
    CU_add_test(suite, "test strdup on empty string", test_refmem_strdup_empty_string);
}
