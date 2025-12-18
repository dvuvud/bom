#include <CUnit/Basic.h>

// Forward declarations of our registration functions from the other test modules
void register_array_allocation_tests();

int main() {
	CU_initialize_registry();

	// Register each file's suite
	register_array_allocation_tests();

	CU_basic_run_tests();

	CU_cleanup_registry();
	return 0;
}
