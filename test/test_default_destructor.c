#include <CUnit/Basic.h>
#include "../src/include/refmem.h"
#include <stdlib.h>

// Simple struct with no pointers
typedef struct {
    int value;
    double data;
} simple_t;

// Struct with one pointer
typedef struct {
    obj *next;
    int value;
} single_ptr_t;

// Struct with multiple pointers
typedef struct {
    obj *left;
    obj *right;
    obj *parent;
    int data;
} multi_ptr_t;

// Counter for tracking destructor calls
static int destructor_call_count = 0;

void counting_destructor(obj *p)
{
    destructor_call_count++;
}

void reset_destructor_count()
{
    destructor_call_count = 0;
}

// Test default destructor with object containing no pointers
void test_default_destructor_no_pointers()
{
    simple_t *s = allocate(sizeof(simple_t), NULL);
    s->value = 42;
    s->data = 3.14;

    retain(s);
    CU_ASSERT_EQUAL(rc(s), 1);

    release(s);

    // Object should be freed, no pointers to release
}

// Test default destructor with NULL pointer field
void test_default_destructor_null_pointer()
{
    single_ptr_t *s = allocate(sizeof(single_ptr_t), NULL);
    s->next = NULL;
    s->value = 100;

    retain(s);
    release(s);
}

// Test default destructor with valid pointer
void test_default_destructor_single_pointer()
{
    reset_destructor_count();

    simple_t *child = allocate(sizeof(simple_t), counting_destructor);
    child->value = 1;
    retain(child);

    single_ptr_t *parent = allocate(sizeof(single_ptr_t), NULL);
    parent->next = child;
    parent->value = 2;

    retain(parent);
    CU_ASSERT_EQUAL(rc(parent), 1);
    CU_ASSERT_EQUAL(rc(child), 1);

    // Release parent should cascade to child via default destructor
    release(parent);

    // Child's destructor should have been called
    CU_ASSERT_EQUAL(destructor_call_count, 1);
}

// Test default destructor with multiple pointers
void test_default_destructor_multiple_pointers()
{
    reset_destructor_count();

    simple_t *left = allocate(sizeof(simple_t), counting_destructor);
    simple_t *right = allocate(sizeof(simple_t), counting_destructor);
    simple_t *parent_obj = allocate(sizeof(simple_t), counting_destructor);

    retain(left);
    retain(right);
    retain(parent_obj);

    multi_ptr_t *root = allocate(sizeof(multi_ptr_t), NULL);
    root->left = left;
    root->right = right;
    root->parent = parent_obj;
    root->data = 42;

    retain(root);

    CU_ASSERT_EQUAL(rc(left), 1);
    CU_ASSERT_EQUAL(rc(right), 1);
    CU_ASSERT_EQUAL(rc(parent_obj), 1);
    CU_ASSERT_EQUAL(rc(root), 1);

    // Release root should cascade to all children
    release(root);

    // All three children destructors should have been called
    CU_ASSERT_EQUAL(destructor_call_count, 3);
}

// Test default destructor doesn't release non-tracked addresses
void test_default_destructor_invalid_pointer()
{
    single_ptr_t *s = allocate(sizeof(single_ptr_t), NULL);

    // Set to arbitrary address that's not a tracked allocation
    s->next = (obj *)0xDEADBEEF;
    s->value = 100;

    retain(s);
    release(s);
}

// Test default destructor with mixed valid and NULL pointers
void test_default_destructor_mixed_pointers()
{
    reset_destructor_count();

    simple_t *left = allocate(sizeof(simple_t), counting_destructor);
    retain(left);

    multi_ptr_t *root = allocate(sizeof(multi_ptr_t), NULL);
    root->left = left;
    root->right = NULL;
    root->parent = NULL;
    root->data = 42;

    retain(root);

    CU_ASSERT_EQUAL(rc(left), 1);
    CU_ASSERT_EQUAL(rc(root), 1);

    release(root);
    
    // Only left's destructor should be called
    CU_ASSERT_EQUAL(destructor_call_count, 1);
}

// Test default destructor with circular reference (requires custom destructor to break cycle)
void test_default_destructor_chain()
{
    reset_destructor_count();

    simple_t *c = allocate(sizeof(simple_t), counting_destructor);
    c->value = 3;
    retain(c);

    single_ptr_t *b = allocate(sizeof(single_ptr_t), NULL);
    b->next = c;
    b->value = 2;
    retain(b);

    single_ptr_t *a = allocate(sizeof(single_ptr_t), NULL);
    a->next = (obj *)b;
    a->value = 1;
    retain(a);

    CU_ASSERT_EQUAL(rc(a), 1);
    CU_ASSERT_EQUAL(rc(b), 1);
    CU_ASSERT_EQUAL(rc(c), 1);

    // Release A should cascade through B to C
    release(a);

    // C's destructor should have been called
    CU_ASSERT_EQUAL(destructor_call_count, 1);
}

// Test default destructor doesn't interfere with custom destructor
void test_default_destructor_vs_custom()
{
    reset_destructor_count();

    // Object with custom destructor
    simple_t *s1 = allocate(sizeof(simple_t), counting_destructor);
    retain(s1);

    // Object with default destructor
    simple_t *s2 = allocate(sizeof(simple_t), NULL);
    retain(s2);

    release(s1);
    release(s2);

    // Only custom destructor should have been called
    CU_ASSERT_EQUAL(destructor_call_count, 1);
}

// Test default destructor with array of pointers
void test_default_destructor_pointer_array()
{
    reset_destructor_count();

    // Allocate array of pointers
    obj **arr = allocate_array(5, sizeof(obj *), NULL);

    // Fill some with objects
    arr[0] = allocate(sizeof(simple_t), counting_destructor);
    retain(arr[0]);

    arr[1] = NULL;

    arr[2] = allocate(sizeof(simple_t), counting_destructor);
    retain(arr[2]);

    arr[3] = NULL;

    arr[4] = allocate(sizeof(simple_t), counting_destructor);
    retain(arr[4]);

    retain(arr);
    release(arr);

    // Three objects should have their destructors called
    CU_ASSERT_EQUAL(destructor_call_count, 3);
}

void register_default_destructor_tests()
{
    CU_pSuite suite = CU_add_suite("Default_Destructor_Tests", NULL, NULL);
    if (suite != NULL)
    {
        CU_add_test(suite, "test no pointers", test_default_destructor_no_pointers);
        CU_add_test(suite, "test NULL pointer", test_default_destructor_null_pointer);
        CU_add_test(suite, "test single pointer", test_default_destructor_single_pointer);
        CU_add_test(suite, "test multiple pointers", test_default_destructor_multiple_pointers);
        CU_add_test(suite, "test invalid pointer", test_default_destructor_invalid_pointer);
        CU_add_test(suite, "test mixed pointers", test_default_destructor_mixed_pointers);
        CU_add_test(suite, "test pointer chain", test_default_destructor_chain);
        CU_add_test(suite, "test custom vs default", test_default_destructor_vs_custom);
        CU_add_test(suite, "test pointer array", test_default_destructor_pointer_array);
    }
}
