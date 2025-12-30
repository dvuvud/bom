#include <CUnit/Basic.h>

// Forward declarations of our registration functions from the other test modules
void register_array_allocation_tests();
void register_queue_tests();
void register_cascade_limit_tests();
void register_cleanup_shutdown_tests();
void register_deallocate_tests();
void register_default_destructor_tests();
void register_hashset_tests();
void register_allocate_tests();
void register_rc_tests();
void register_retain_release_tests();
void register_strdup_tests();

int main() {
    CU_initialize_registry();

    // Register each file's suite
    register_array_allocation_tests();
    register_queue_tests();
    register_cascade_limit_tests();
    register_cleanup_shutdown_tests();
    register_deallocate_tests();
    register_default_destructor_tests();
    register_hashset_tests();
    register_allocate_tests();
    register_rc_tests();
    register_retain_release_tests();
    register_strdup_tests();

    CU_basic_run_tests();

    CU_cleanup_registry();

    return 0;
}
