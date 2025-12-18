#include <CUnit/Basic.h>
#include "../src/include/queue.h"
#include <stdlib.h>

struct queue_node {
	void *data;
	struct queue_node *next;
};

void test_queue_basic_operations() {
	queue_t q = { .head = NULL, .tail = NULL, .count = 0 };

	int val1 = 100;
	int val2 = 200;

	CU_ASSERT_EQUAL(queue_push(&q, &val1), 0);
	CU_ASSERT_EQUAL(q.count, 1);
	CU_ASSERT_PTR_NOT_NULL(q.head);
	CU_ASSERT_PTR_EQUAL(q.head->data, &val1);

	CU_ASSERT_EQUAL(queue_push(&q, &val2), 0);
	CU_ASSERT_EQUAL(q.count, 2);
	CU_ASSERT_PTR_EQUAL(q.tail->data, &val2);

	queue_pop(&q);
	CU_ASSERT_EQUAL(q.count, 1);
	CU_ASSERT_PTR_EQUAL(q.head->data, &val2);

	queue_pop(&q);
	CU_ASSERT_EQUAL(q.count, 0);
	CU_ASSERT_PTR_NULL(q.head);
	CU_ASSERT_PTR_NULL(q.tail);

	queue_clear(&q);
}

void test_queue_clear() {
	queue_t q = { NULL, NULL, 0 };
	int dummy = 42;

	for (int i = 0; i < 5; i++) {
		queue_push(&q, &dummy);
	}

	CU_ASSERT_EQUAL(q.count, 5);
	queue_clear(&q);
	CU_ASSERT_EQUAL(q.count, 0);
	CU_ASSERT_PTR_NULL(q.head);
}

void register_queue_tests() {
	CU_pSuite suite = CU_add_suite("Queue_Internal_Tests", NULL, NULL);
	if (suite != NULL) {
		CU_add_test(suite, "test push and pop logic", test_queue_basic_operations);
		CU_add_test(suite, "test clear functionality", test_queue_clear);
	}
}
