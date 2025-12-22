#include <CUnit/Basic.h>

// Forward declarations of our registration functions from the other test modules
void register_array_allocation_tests();
void register_queue_tests();
void register_cascade_limit_tests();
void register_cleanup_shutdown_tests();

int main() {
	CU_initialize_registry();

	// Register each file's suite
	register_array_allocation_tests();
	register_queue_tests();
	register_cascade_limit_tests();
	register_cleanup_shutdown_tests();

	CU_basic_run_tests();

	CU_cleanup_registry();
	return 0;
}
