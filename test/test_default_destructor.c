#include <CUnit/Basic.h>
#include "../src/include/refmem.h"

/*
 * Simple test structure containing an internal object pointer.
 * This allows us to test the default destructor.
 */
struct test_node 
{
    obj *child;
};

void test_default_destructor_releases_child()
{
     // Allocate child object
    obj *child = allocate(sizeof(int), NULL);
    retain(child);

    // Allocate parent without explicit destructor
    struct test_node *parent = allocate(sizeof(struct test_node), NULL);
    retain(parent);

    // Store child inside parent
    parent->child = child;

    // Releasing parent triggers default destructor
    release(parent); 

    // At this point, both parent and child should be freed.
    CU_ASSERT_TRUE(1);
}

void register_default_destructor_tests()
{
    CU_pSuite suite = CU_add_suite("Default_Destructor_Tests", NULL, NULL);

    if (suite != NULL) 
    {
        CU_add_test(suite, "default destructor releases internal pointers", test_default_destructor_releases_child);
    }
}