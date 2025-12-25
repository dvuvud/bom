#include <CUnit/Basic.h>
#include "../src/include/hashset.h"
#include <stdint.h>

// Test basic add and contains
void test_hashset_add_contains()
{
    // Clean state
    hashset_cleanup();

    void *addr1 = (void *)0x1000;
    void *addr2 = (void *)0x2000;
    void *addr3 = (void *)0x3000;

    // Initially should not contain anything
    CU_ASSERT_FALSE(hashset_contains(addr1));
    CU_ASSERT_FALSE(hashset_contains(addr2));

    // Add addresses
    hashset_add(addr1);
    hashset_add(addr2);

    // Should now contain them
    CU_ASSERT_TRUE(hashset_contains(addr1));
    CU_ASSERT_TRUE(hashset_contains(addr2));
    CU_ASSERT_FALSE(hashset_contains(addr3));

    hashset_cleanup();
}

// Test adding NULL
void test_hashset_add_null()
{
    hashset_cleanup();

    hashset_add(NULL);
    CU_ASSERT_FALSE(hashset_contains(NULL));

    hashset_cleanup();
}

// Test contains on NULL
void test_hashset_contains_null()
{
    hashset_cleanup();

    CU_ASSERT_FALSE(hashset_contains(NULL));

    hashset_cleanup();
}

// Test remove functionality
void test_hashset_remove()
{
    hashset_cleanup();

    void *addr1 = (void *)0x1000;
    void *addr2 = (void *)0x2000;

    hashset_add(addr1);
    hashset_add(addr2);

    CU_ASSERT_TRUE(hashset_contains(addr1));
    CU_ASSERT_TRUE(hashset_contains(addr2));

    // Remove addr1
    hashset_remove(addr1);

    CU_ASSERT_FALSE(hashset_contains(addr1));
    CU_ASSERT_TRUE(hashset_contains(addr2));

    // Remove addr2
    hashset_remove(addr2);

    CU_ASSERT_FALSE(hashset_contains(addr1));
    CU_ASSERT_FALSE(hashset_contains(addr2));

    hashset_cleanup();
}

// Test removing NULL
void test_hashset_remove_null()
{
    hashset_cleanup();

    // Should not crash
    hashset_remove(NULL);

    hashset_cleanup();
}

// Test removing non-existent address
void test_hashset_remove_nonexistent()
{
    hashset_cleanup();

    void *addr1 = (void *)0x1000;
    void *addr2 = (void *)0x2000;

    hashset_add(addr1);

    // Remove something not in the set
    hashset_remove(addr2);

    // addr1 should still be there
    CU_ASSERT_TRUE(hashset_contains(addr1));

    hashset_cleanup();
}

// Test duplicate adds
void test_hashset_duplicate_add()
{
    hashset_cleanup();

    void *addr = (void *)0x1000;

    hashset_add(addr);
    hashset_add(addr);
    hashset_add(addr);

    CU_ASSERT_TRUE(hashset_contains(addr));

    // Remove once should remove it
    hashset_remove(addr);
    CU_ASSERT_FALSE(hashset_contains(addr));

    hashset_cleanup();
}

// Test many addresses
void test_hashset_many_addresses()
{
    hashset_cleanup();

#define NUM_ADDRS 1000
    void *addrs[NUM_ADDRS];

    // Generate addresses
    for (int i = 0; i < NUM_ADDRS; i++) {
        addrs[i] = (void *)(uintptr_t)(0x1000 + i * 0x100);
        hashset_add(addrs[i]);
    }

    // Verify all are present
    for (int i = 0; i < NUM_ADDRS; i++) {
        CU_ASSERT_TRUE(hashset_contains(addrs[i]));
    }

    // Remove every other one
    for (int i = 0; i < NUM_ADDRS; i += 2) {
        hashset_remove(addrs[i]);
    }

    // Verify correct ones remain
    for (int i = 0; i < NUM_ADDRS; i++) {
        if (i % 2 == 0) {
            CU_ASSERT_FALSE(hashset_contains(addrs[i]));
        } else {
            CU_ASSERT_TRUE(hashset_contains(addrs[i]));
        }
    }

    hashset_cleanup();
}

// Test cleanup
void test_hashset_cleanup()
{
    hashset_cleanup();

    void *addr1 = (void *)0x1000;
    void *addr2 = (void *)0x2000;

    hashset_add(addr1);
    hashset_add(addr2);

    CU_ASSERT_TRUE(hashset_contains(addr1));
    CU_ASSERT_TRUE(hashset_contains(addr2));

    hashset_cleanup();

    // After cleanup, nothing should be present
    CU_ASSERT_FALSE(hashset_contains(addr1));
    CU_ASSERT_FALSE(hashset_contains(addr2));
}

// Test that hashset can be reused after cleanup
void test_hashset_reuse_after_cleanup()
{
    hashset_cleanup();

    void *addr1 = (void *)0x1000;

    hashset_add(addr1);
    CU_ASSERT_TRUE(hashset_contains(addr1));

    hashset_cleanup();

    // Reuse after cleanup
    void *addr2 = (void *)0x2000;
    hashset_add(addr2);
    CU_ASSERT_TRUE(hashset_contains(addr2));
    CU_ASSERT_FALSE(hashset_contains(addr1));

    hashset_cleanup();
}

// Test hash collisions
void test_hashset_collisions()
{
    hashset_cleanup();

    // These addresses potentially collide
    void *addr1 = (void *)0x1000;
    void *addr2 = (void *)0x1008;
    void *addr3 = (void *)0x1010;

    hashset_add(addr1);
    hashset_add(addr2);
    hashset_add(addr3);

    // All should be findable
    CU_ASSERT_TRUE(hashset_contains(addr1));
    CU_ASSERT_TRUE(hashset_contains(addr2));
    CU_ASSERT_TRUE(hashset_contains(addr3));

    // Remove middle one
    hashset_remove(addr2);

    CU_ASSERT_TRUE(hashset_contains(addr1));
    CU_ASSERT_FALSE(hashset_contains(addr2));
    CU_ASSERT_TRUE(hashset_contains(addr3));

    hashset_cleanup();
}

void register_hashset_tests()
{
    CU_pSuite suite = CU_add_suite("Hashset_Tests", NULL, NULL);
    if (suite != NULL)
    {
        CU_add_test(suite, "test add and contains", test_hashset_add_contains);
        CU_add_test(suite, "test add NULL", test_hashset_add_null);
        CU_add_test(suite, "test contains NULL", test_hashset_contains_null);
        CU_add_test(suite, "test remove", test_hashset_remove);
        CU_add_test(suite, "test remove NULL", test_hashset_remove_null);
        CU_add_test(suite, "test remove nonexistent", test_hashset_remove_nonexistent);
        CU_add_test(suite, "test duplicate add", test_hashset_duplicate_add);
        CU_add_test(suite, "test many addresses", test_hashset_many_addresses);
        CU_add_test(suite, "test cleanup", test_hashset_cleanup);
        CU_add_test(suite, "test reuse after cleanup", test_hashset_reuse_after_cleanup);
        CU_add_test(suite, "test hash collisions", test_hashset_collisions);
    }
}
