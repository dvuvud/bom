#include <CUnit/Basic.h>
#include <stdlib.h>

#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"

#define No_Buckets 17

int init_suite(void) {
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void) {
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

//// -------------------------- HASH TABLE TESTS --------------------------

void test_create_destroy() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_PTR_NOT_NULL(ht);
    ioopm_hash_table_destroy(ht);
}

// Hash function definitions
static int string_sum_hash(elem_t e){
    char *str = e.p;
    int result = 0;
    do {
        result += *str;
    } while (*++str != '\0');
    return result;
}

void test_create_destroy_other_hf(){
    //tests create and destroy on a hash table that does not use the default hash function
    ioopm_hash_table_t *hf = ioopm_hash_table_create(*string_sum_hash,str_equiv);
    ioopm_hash_table_insert(hf, ptr_elem("a"), int_elem(3));
    ioopm_hash_table_destroy(hf);
}

void test_insert_once() { //test with a fresh key
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL); //Create empty new hash table
    elem_t k = int_elem(1);
    elem_t v = ptr_elem("a");
    ioopm_option_t ans = ioopm_hash_table_lookup(ht,k); //Kolla först att den inte hittar i en tom ht
    CU_ASSERT_EQUAL(ans.success,0); //Kolla att .success är falskt
    ioopm_hash_table_insert(ht, k, v); //Stoppa in ett element i ht
    ioopm_option_t ans2 = ioopm_hash_table_lookup(ht,k);
    CU_ASSERT_PTR_EQUAL(ans2.value.p, "a");
    CU_ASSERT_NOT_EQUAL(ans2.success,0);
    ioopm_hash_table_destroy(ht);
}

void test_insert_key_exists() { //Skriv över tidigare värdet
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL); //Create empty new hash table
    elem_t k = int_elem(1);
    elem_t v = ptr_elem("a");
    ioopm_hash_table_insert(ht, k, v);
    ioopm_option_t ans = ioopm_hash_table_lookup(ht,k); 
    CU_ASSERT_PTR_EQUAL(ans.value.p, "a");
    CU_ASSERT_NOT_EQUAL(ans.success,0);
    
    elem_t v2 = ptr_elem("b");
    ioopm_hash_table_insert(ht, k, v2); //overwrite previous value
    ioopm_option_t ans2 = ioopm_hash_table_lookup(ht,k); 
    CU_ASSERT_PTR_EQUAL(ans2.value.p, "b");
    CU_ASSERT_NOT_EQUAL(ans2.success,0);
    ioopm_hash_table_destroy(ht);
}


void test_insert_same_bucket() { //test with a key that hashes to the same bucket as the previous
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL); //Create empty new hash table
    elem_t k1 = int_elem(1);
    elem_t k2 = int_elem(1 + No_Buckets);
    elem_t v1 = ptr_elem("a");
    elem_t v2 = ptr_elem("b");
    ioopm_hash_table_insert(ht, k1, v1); //Stoppa in ett element i ht
    ioopm_hash_table_insert(ht, k2, v2); //Stoppa in ett element i ht
    ioopm_option_t ans1 = ioopm_hash_table_lookup(ht,k1);
    CU_ASSERT_PTR_EQUAL(ans1.value.p, "a");
    CU_ASSERT_NOT_EQUAL(ans1.success,0);
    ioopm_option_t ans2 = ioopm_hash_table_lookup(ht,k2);
    CU_ASSERT_PTR_EQUAL(ans2.value.p, "b");
    CU_ASSERT_NOT_EQUAL(ans2.success,0);
    ioopm_hash_table_destroy(ht);
}

void test_lookup_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    for (int i = 0; i < No_Buckets; ++i){  /// 18 is 1 bigger than size of hash table
       CU_ASSERT_EQUAL(ioopm_hash_table_lookup(ht, int_elem(i)).success, 0);
    }
    ioopm_hash_table_destroy(ht);
}

void test_remove_key_empty() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL); //Create empty new hash table
    elem_t k = int_elem(1);
    ioopm_option_t ans = ioopm_hash_table_remove(ht, k); //try to remove key k
    CU_ASSERT_EQUAL(ans.success, 0);
    ioopm_option_t ans2 = ioopm_hash_table_lookup(ht,k); 
    CU_ASSERT_EQUAL(ans2.success, 0);
    ioopm_hash_table_destroy(ht);
}

void test_remove_only_key() {
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL); //Create empty new hash table
    elem_t k = int_elem(1);
    elem_t v = ptr_elem("a");
    ioopm_hash_table_insert(ht,k,v);
    ioopm_option_t ans = ioopm_hash_table_remove(ht, k); //try to remove key k
    CU_ASSERT_TRUE(ans.success); 
    CU_ASSERT_PTR_EQUAL(ans.value.p, "a");
    ioopm_hash_table_destroy(ht);
}

void test_remove_existing_key() { //Tests remove within the same bucket (three cases)
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL); //Create empty new hash table
    elem_t k1 = int_elem(1);
    elem_t k2 = int_elem(1 + No_Buckets);
    elem_t k3 = int_elem(1 + No_Buckets*2);
    elem_t v1 = ptr_elem("a");
    elem_t v2 = ptr_elem("b");
    elem_t v3 = ptr_elem("c");
    //Fill up hash table
    ioopm_hash_table_insert(ht, k1, v1);
    ioopm_hash_table_insert(ht, k2, v2);
    ioopm_hash_table_insert(ht, k3, v3);

    ioopm_option_t ans1 = ioopm_hash_table_remove(ht, k1); //try to remove key k1 (first element)
    CU_ASSERT_NOT_EQUAL(ans1.success, 0); 
    CU_ASSERT_PTR_EQUAL(ans1.value.p, "a");
    ioopm_hash_table_insert(ht, k1, v1); // add back

    ioopm_option_t ans2 = ioopm_hash_table_remove(ht, k2); //try to remove key k2 (middle element)
    CU_ASSERT_NOT_EQUAL(ans2.success, 0); 
    CU_ASSERT_PTR_EQUAL(ans2.value.p, "b");
    ioopm_hash_table_insert(ht, k2, v2); // add back

    ioopm_option_t ans3 = ioopm_hash_table_remove(ht, k3); //try to remove key k1 (last element)
    CU_ASSERT_NOT_EQUAL(ans3.success, 0); 
    CU_ASSERT_PTR_EQUAL(ans3.value.p, "c");

    ioopm_hash_table_destroy(ht);
}

void test_size_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_size_one(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("a"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_destroy(ht);
}

void test_size_plural(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("a"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("b"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("c"));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);
    ioopm_hash_table_destroy(ht);
}

void test_is_empty_true(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_NOT_EQUAL(ioopm_hash_table_is_empty(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_is_empty_false(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("a"));
    CU_ASSERT_EQUAL(ioopm_hash_table_is_empty(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_clear_empty_ht(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_NOT_EQUAL(ioopm_hash_table_is_empty(ht), 0);
    ioopm_hash_table_clear(ht);
    CU_ASSERT_NOT_EQUAL(ioopm_hash_table_is_empty(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_clear_non_empty_ht(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    ioopm_hash_table_insert(ht,int_elem(0),ptr_elem("a"));
    CU_ASSERT_EQUAL(ioopm_hash_table_is_empty(ht), 0);
    ioopm_hash_table_clear(ht);
    CU_ASSERT_NOT_EQUAL(ioopm_hash_table_is_empty(ht), 0);
    ioopm_hash_table_destroy(ht);
}

void test_keys_empty(){ 
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(keys));
    ioopm_linked_list_destroy(keys);
    ioopm_hash_table_destroy(ht);
}

void test_keys_non_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    int keys[5] = {1, 2, 3, 4, 5};
    bool found[5] ={false};
    for(int i = 0; i < (sizeof(keys)/sizeof(keys[0])); i++){
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem("a"));
    }
    ioopm_list_t *res = ioopm_hash_table_keys(ht);
    ioopm_link_t *lnk = res->first;
    while(lnk){
        for(int j = 0; j < 5; j++) {
            if(lnk->element.i==keys[j]){
                found[j] = true;
            }
        }
        lnk = lnk->next;
    }
    for(int i = 0; i < 5; i++) {
        CU_ASSERT_TRUE(found[i]);
    }
    ioopm_linked_list_destroy(res);
    ioopm_hash_table_destroy(ht);
}

void test_values_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    ioopm_list_t *values = ioopm_hash_table_values(ht);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(values));
    ioopm_linked_list_destroy(values);
    ioopm_hash_table_destroy(ht);
}

void test_values_non_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    int keys[5] = {1, 2, 3, 4, 5};
    char *values[5] = {"one", "two", "three", "four", "five"};
    bool found[5] ={false};
    for(int i = 0; i < (sizeof(keys)/sizeof(keys[0])); i++){
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }
    ioopm_list_t *res_keys = ioopm_hash_table_keys(ht);
    ioopm_link_t *r_keys = res_keys->first;
    ioopm_list_t *res_values = ioopm_hash_table_values(ht);
    ioopm_link_t *r_values = res_values->first;
    for(int i = 0; i < 5; i++){
        if(strcmp(r_values->element.p,values[i])==0 && r_keys->element.i==keys[i]){
            found[i] = true;
        }
        r_values = r_values->next;
        r_keys = r_keys->next;
    }
    for(int i = 0; i < 5; i++) {
        CU_ASSERT_TRUE(found[i]);
    }
    ioopm_linked_list_destroy(res_keys);
    ioopm_linked_list_destroy(res_values);
    ioopm_hash_table_destroy(ht);
}

void test_has_key_false(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(1)));
    
    int keys[5] = {1, 2, 3, 4, 5};

    for(int i = 0; i < (sizeof(keys)/sizeof(keys[0])); i++){
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem("a"));
    }
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(6)));
    ioopm_hash_table_destroy(ht);
}

void test_has_key_true(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(1)));
    int keys[5] = {1, 2, 3, 4, 5};
    for(int i = 0; i < (sizeof(keys)/sizeof(keys[0])); i++){
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem("a"));
    }
    for(int i = 1; i < 6; i++) {
        CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, int_elem(i)));
    }
    ioopm_hash_table_destroy(ht);
}

void test_has_value_false(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("eight")));
    int keys[5] = {1, 2, 3, 4, 5};
    char *values[5] = {"one", "two", "three", "four", "five"};
    for(int i = 0; i < (sizeof(keys)/sizeof(keys[0])); i++){
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }
    CU_ASSERT_FALSE(ioopm_hash_table_has_value(ht, ptr_elem("eight")));
    ioopm_hash_table_destroy(ht);
}

void test_has_value_true(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    int keys[5] = {1, 2, 3, 4, 5};
    char *values[5] = {"one", "two", "three", "four", "five"};
    for(int i = 0; i < 5; i++){
        ioopm_hash_table_insert(ht, int_elem(keys[i]), ptr_elem(values[i]));
    }
    for(int i = 0; i < 5; i++){
        CU_ASSERT_TRUE(ioopm_hash_table_has_value(ht, ptr_elem(values[i])));
    }
    
    ioopm_hash_table_destroy(ht);
}

//predicate for all function
static bool all_equals(elem_t key_ignored, elem_t value, void *wanted){ //Ändra till entry_t
    // return strcmp(value, wanted) == 0;
    char *x = wanted;
    return strcmp(value.p, x)==0;
}

void test_hash_table_all_true(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    char *value = "a";
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, *all_equals, value));
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem(value));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem(value));
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, *all_equals, value));
    ioopm_hash_table_destroy(ht);
}

void test_hash_table_all_false(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    char *value = "a";
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem(value));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem(value));
    char *not_value = "b";
    CU_ASSERT_FALSE(ioopm_hash_table_all(ht, *all_equals, not_value));
    ioopm_hash_table_destroy(ht);
}


static void same_value(elem_t key, elem_t *value, void *new_value){
    *value = ptr_elem(new_value);
}

void test_apply_to_all_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    char *value = "a";
    ioopm_hash_table_apply_to_all(ht, same_value, value);
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, all_equals, value));
    ioopm_hash_table_destroy(ht);
}

void test_apply_to_all_non_empty(){
    ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL,NULL);
    char *value = "a";
    ioopm_hash_table_insert(ht, int_elem(1), ptr_elem("a"));
    ioopm_hash_table_insert(ht, int_elem(2), ptr_elem("b"));
    ioopm_hash_table_apply_to_all(ht, same_value, value);
    CU_ASSERT_TRUE(ioopm_hash_table_all(ht, all_equals, value));
    ioopm_hash_table_destroy(ht);
}



//// -------------------------- LINKED LIST TESTS --------------------------


void test_create_destroy_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_PTR_NOT_NULL(ll);
    ioopm_linked_list_destroy(ll);
}

void test_append_one_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(1));
    CU_ASSERT_EQUAL(ll->first->element.i, 1);
    CU_ASSERT_EQUAL(ll->last->element.i, 1);
    ioopm_linked_list_destroy(ll);
}

void test_append_plural_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(1));
    CU_ASSERT_EQUAL(ll->last->element.i, 1);
    ioopm_linked_list_append(ll,int_elem(2));
    CU_ASSERT_EQUAL(ll->last->element.i, 2);
    ioopm_linked_list_destroy(ll);
}

void test_prepend_one_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_prepend(ll,int_elem(1));
    CU_ASSERT_EQUAL(ll->first->element.i, 1);
    CU_ASSERT_EQUAL(ll->last->element.i, 1);
    ioopm_linked_list_destroy(ll);
}

void test_prepend_plural_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_prepend(ll,int_elem(1));
    CU_ASSERT_EQUAL(ll->first->element.i, 1);
    ioopm_linked_list_prepend(ll,int_elem(2));
    CU_ASSERT_EQUAL(ll->first->element.i, 2);
    CU_ASSERT_EQUAL(ll->last->element.i, 1);
    ioopm_linked_list_destroy(ll);
}

void test_insert_invalid_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_insert(ll,5,int_elem(1));
    CU_ASSERT_PTR_NULL(ll->first);
    ioopm_linked_list_insert(ll,-5,int_elem(1));
    CU_ASSERT_PTR_NULL(ll->first);
    ioopm_linked_list_destroy(ll);
}

void test_insert_once_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_insert(ll,0,int_elem(1));
    CU_ASSERT_EQUAL(ll->first->element.i,1);
    CU_ASSERT_PTR_NULL(ll->first->next);
    CU_ASSERT_EQUAL(ll->last->element.i,1);
    CU_ASSERT_PTR_NULL(ll->last->next);
    ioopm_linked_list_destroy(ll);
}

void test_insert_plural_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_insert(ll,0,int_elem(1)); //insert in beginning
    CU_ASSERT_EQUAL(ll->first->element.i,1);
    ioopm_linked_list_insert(ll,1,int_elem(2)); //insert in end
    CU_ASSERT_EQUAL(ll->last->element.i,2);
    ioopm_linked_list_insert(ll,1,int_elem(3)); //insert in middle
    CU_ASSERT_EQUAL(ll->first->next->element.i,3);
    ioopm_linked_list_destroy(ll);
}

void test_remove_invalid_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_remove(ll,1);
    CU_ASSERT_PTR_NULL(ll->first);
    ioopm_linked_list_append(ll,int_elem(1));
    ioopm_linked_list_remove(ll,5);
    CU_ASSERT_EQUAL(ll->first->element.i,1);
    ioopm_linked_list_destroy(ll);
}

void test_remove_once_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(1));
    elem_t ans = ioopm_linked_list_remove(ll,0);
    CU_ASSERT_EQUAL(ans.i, 1);
    CU_ASSERT_PTR_NULL(ll->first);
    ioopm_linked_list_destroy(ll);
}

void test_remove_many_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(10));
    ioopm_linked_list_append(ll,int_elem(20));
    ioopm_linked_list_append(ll,int_elem(30));
    elem_t ans1 = ioopm_linked_list_remove(ll,1); //remove middle element
    CU_ASSERT_EQUAL(ans1.i, 20);
    elem_t ans2 = ioopm_linked_list_remove(ll,1); //remove last element
    CU_ASSERT_EQUAL(ans2.i, 30);
    elem_t ans3 = ioopm_linked_list_remove(ll,0); //remove first element
    CU_ASSERT_EQUAL(ans3.i, 10);

    CU_ASSERT_PTR_NULL(ll->first);
    ioopm_linked_list_destroy(ll);
}

void test_get_invalid_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,0).i,0);
    ioopm_linked_list_append(ll,int_elem(1));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,-1).i,0);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,8).i,0);
    ioopm_linked_list_destroy(ll);
}

void test_get_one_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,0).i,10);
    ioopm_linked_list_destroy(ll);
}

void test_get_many_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(20));
    ioopm_linked_list_append(ll, int_elem(30));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,0).i,10);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,1).i,20);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(ll,2).i,30);
    ioopm_linked_list_destroy(ll);
}

void test_contains_false_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_FALSE(ioopm_linked_list_contains(ll,int_elem(10)));
    ioopm_linked_list_append(ll, int_elem(10));
    CU_ASSERT_FALSE(ioopm_linked_list_contains(ll,int_elem(100)));
    ioopm_linked_list_destroy(ll);
}

void test_contains_true_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll,int_elem(10)));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll,int_elem(100)));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(ll,int_elem(1000)));
    ioopm_linked_list_destroy(ll);
}

void test_size_empty_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(ll),0);
    ioopm_linked_list_destroy(ll);
}

void test_size_nonempty_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(ll), 3);
    ioopm_linked_list_destroy(ll);
}

void test_is_empty_true_ll(){ 
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ll));
    ioopm_linked_list_destroy(ll);
}

void test_is_empty_false_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(ll));
    ioopm_linked_list_destroy(ll);
}

void test_clear_empty_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_clear(ll);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ll));
    ioopm_linked_list_destroy(ll);
}

void test_clear_non_empty_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    ioopm_linked_list_clear(ll);
    CU_ASSERT_TRUE(ioopm_linked_list_is_empty(ll));
    ioopm_linked_list_destroy(ll);
}

static bool all_even_ll(elem_t ignored, elem_t value, void *extra){ //Kollar om alla är jämna
    return value.i % 2 == 0;
}

void test_all_false_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_TRUE(ioopm_linked_list_all(ll, *all_even_ll, NULL));
    ioopm_linked_list_append(ll, int_elem(11));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    CU_ASSERT_FALSE(ioopm_linked_list_all(ll, *all_even_ll, NULL));
    ioopm_linked_list_destroy(ll);
}

void test_all_true_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_TRUE(ioopm_linked_list_all(ll, *all_even_ll, NULL));
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    CU_ASSERT_TRUE(ioopm_linked_list_all(ll, *all_even_ll, NULL));
    ioopm_linked_list_destroy(ll);
}

void test_any_false_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    CU_ASSERT_FALSE(ioopm_linked_list_any(ll, *all_even_ll, NULL));
    ioopm_linked_list_append(ll, int_elem(11));
    ioopm_linked_list_append(ll, int_elem(101));
    ioopm_linked_list_append(ll, int_elem(1001));
    CU_ASSERT_FALSE(ioopm_linked_list_any(ll, *all_even_ll, NULL));
    ioopm_linked_list_destroy(ll);
}

void test_any_true_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1001));
    CU_ASSERT_TRUE(ioopm_linked_list_any(ll, *all_even_ll, NULL));
    ioopm_linked_list_destroy(ll);
}

static void make_even_ll(elem_t value, elem_t *ignored, void *extra){
    value.i = value.i*2;
}

void test_apply_to_all_empty_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_apply_to_all(ll, *make_even_ll, NULL);
    CU_ASSERT_FALSE(ioopm_linked_list_any(ll, *all_even_ll, NULL));
    ioopm_linked_list_destroy(ll);
}

void test_apply_to_all_non_empty_ll(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll, int_elem(10));
    ioopm_linked_list_append(ll, int_elem(100));
    ioopm_linked_list_append(ll, int_elem(1000));
    ioopm_linked_list_apply_to_all(ll, *make_even_ll, NULL);
    CU_ASSERT_TRUE(ioopm_linked_list_all(ll, *all_even_ll, NULL));
    ioopm_linked_list_destroy(ll);
}

//// -------------------------- ITERATOR TESTS --------------------------

void test_has_next_false_it(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_FALSE(ioopm_iterator_has_next(it));
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_has_next_true_it(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(10));
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(it));
    ioopm_iterator_destroy(it);
    ioopm_linked_list_append(ll,int_elem(10));
    it = ioopm_iterator_create(ll);
    CU_ASSERT_TRUE(ioopm_iterator_has_next(it));
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_next_invalid(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_EQUAL(ioopm_iterator_next(it).i,0);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_next_possible(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(10));
    ioopm_linked_list_append(ll,int_elem(20));
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_EQUAL(ioopm_iterator_next(it).i,10);
    CU_ASSERT_EQUAL(ioopm_iterator_next(it).i,20);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_reset_empty(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    ioopm_iterator_reset(it);
    CU_ASSERT_PTR_NULL(it->current);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_reset_nonempty(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(10));
    ioopm_linked_list_append(ll,int_elem(20));
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    ioopm_iterator_next(it);
    CU_ASSERT_EQUAL(ioopm_iterator_current(it).i, 20);
    ioopm_iterator_reset(it);
    CU_ASSERT_EQUAL(ioopm_iterator_current(it).i, 10);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_current_empty(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_EQUAL(ioopm_iterator_current(it).i, 0);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_current_nonempty(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_linked_list_append(ll,int_elem(10));
    ioopm_linked_list_append(ll,int_elem(20));
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_EQUAL(ioopm_iterator_current(it).i, 10);
    ioopm_iterator_next(it);
    CU_ASSERT_EQUAL(ioopm_iterator_current(it).i, 20);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

void test_iterate_create_destroy(){
    ioopm_list_t *ll = ioopm_linked_list_create(int_equiv);
    ioopm_list_iterator_t *it = ioopm_iterator_create(ll);
    CU_ASSERT_PTR_NOT_NULL(it);
    ioopm_iterator_destroy(it);
    ioopm_linked_list_destroy(ll);
}

int main() {
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite1 = CU_add_suite("Inlupp 1 ticket 1 test suite", init_suite, clean_suite);
    CU_pSuite my_test_suite2 = CU_add_suite("Inlupp 1 ticket 2 test suite", init_suite, clean_suite);
    CU_pSuite my_test_suite3 = CU_add_suite("Inlupp 1 ticket 3 test suite", init_suite, clean_suite);
    if (my_test_suite1 == NULL || my_test_suite2 == NULL || my_test_suite3 == NULL) {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
    if (
        // -------------------------- HASH TABLE TESTS --------------------------
        //Test create & destroy
        (CU_add_test(my_test_suite1, "create_destroy test", test_create_destroy) == NULL) ||
        // Test create & destroy with different hash func
        (CU_add_test(my_test_suite1, "create destroy other hash function", test_create_destroy_other_hf) == NULL) ||
        //Test insertion and lookup
        (CU_add_test(my_test_suite1, "insert once test", test_insert_once) == NULL) ||
        (CU_add_test(my_test_suite1, "insert an existing key test", test_insert_key_exists) == NULL) ||
        (CU_add_test(my_test_suite1, "insert in to the same bucket", test_insert_same_bucket) == NULL) ||
        (CU_add_test(my_test_suite1, "look up in empty hash table", test_lookup_empty) == NULL) ||
        //Test remove
        (CU_add_test(my_test_suite1, "try to remove from empty hash table", test_remove_key_empty) == NULL) ||
        (CU_add_test(my_test_suite1, "remove only entry in hash table", test_remove_only_key) == NULL) ||
        (CU_add_test(my_test_suite1, "remove existing entries in hash table", test_remove_existing_key) == NULL) ||
        //Test size
        (CU_add_test(my_test_suite2, "size of empty hash table is zero", test_size_empty) == NULL) ||
        (CU_add_test(my_test_suite2, "size of hash table with one entry", test_size_one) == NULL) ||
        (CU_add_test(my_test_suite2, "size of hash table with several entries", test_size_plural) == NULL) ||
        //Test is empty
        (CU_add_test(my_test_suite2, "empty hash table is empty", test_is_empty_true) == NULL) ||
        (CU_add_test(my_test_suite2, "non-empty hash table is non-empty", test_is_empty_false) == NULL) ||
        //Test clear
        (CU_add_test(my_test_suite2, "clear empty hash table", test_clear_empty_ht) == NULL) ||
        (CU_add_test(my_test_suite2, "clear non-empty hash table", test_clear_non_empty_ht) == NULL) ||
        //Test find all keys
        (CU_add_test(my_test_suite2, "keys of empty hash table", test_keys_empty) == NULL) ||
        (CU_add_test(my_test_suite2, "keys of non-empty hash table", test_keys_non_empty) == NULL) ||
        //Test find all values
        (CU_add_test(my_test_suite2, "values of empty hash table", test_values_empty) == NULL) ||
        (CU_add_test(my_test_suite2, "values of non-empty hash table", test_values_non_empty) == NULL) ||
        //Test has key/hash table any
        (CU_add_test(my_test_suite2, "hash table does not have key", test_has_key_false) == NULL) ||
        (CU_add_test(my_test_suite2, "hash table has key", test_has_key_true) == NULL) ||
        //Test has value
        (CU_add_test(my_test_suite2, "hash table does not have value", test_has_value_false) == NULL) ||
        (CU_add_test(my_test_suite2, "hash table has key", test_has_value_true) == NULL) ||
        // Test all
        (CU_add_test(my_test_suite2, "hash table all true", test_hash_table_all_true) == NULL) ||
        (CU_add_test(my_test_suite2, "hash table all false", test_hash_table_all_false) == NULL) ||
        //Test apply function for all
        (CU_add_test(my_test_suite2, "hash table apply to all of empty hash table", test_apply_to_all_empty) == NULL) ||
        (CU_add_test(my_test_suite2, "hash table apply to all", test_apply_to_all_non_empty) == NULL) ||
        //// -------------------------- LINKED LIST TESTS --------------------------
        //create destroy
        (CU_add_test(my_test_suite3, "create destroy linked list", test_create_destroy_ll) == NULL) ||
        //append
        (CU_add_test(my_test_suite3, "append once", test_append_one_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "append plural", test_append_plural_ll) == NULL) ||
        //prepend
        (CU_add_test(my_test_suite3, "prepend once", test_prepend_one_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "prepend plural", test_prepend_plural_ll) == NULL) ||
        //insert
        (CU_add_test(my_test_suite3, "insert invalid index", test_insert_invalid_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "insert once", test_insert_once_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "insert many", test_insert_plural_ll) == NULL) ||
        //remove
        (CU_add_test(my_test_suite3, "remove invalid index", test_remove_invalid_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "remove once", test_remove_once_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "remove many", test_remove_many_ll) == NULL) ||
        //get
        (CU_add_test(my_test_suite3, "get invalid index", test_get_invalid_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "get once", test_get_one_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "get many", test_get_many_ll) == NULL) ||
        //contains
        (CU_add_test(my_test_suite3, "contains false", test_contains_false_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "contains true", test_contains_true_ll) == NULL) ||
        //size
        (CU_add_test(my_test_suite3, "size of empty linked list", test_size_empty_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "size of non-empty linked list", test_size_nonempty_ll) == NULL) ||
        //is empty
        (CU_add_test(my_test_suite3, "empty linked list is empty", test_is_empty_true_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "non-empty linked list is not empty", test_is_empty_false_ll) == NULL) ||
        //clear
        (CU_add_test(my_test_suite3, "clear empty list", test_clear_empty_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "clear non-empty list", test_clear_non_empty_ll) == NULL) ||
        //all
        (CU_add_test(my_test_suite3, "all of list is false", test_all_false_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "all of list is true", test_all_true_ll) == NULL) ||
        //any
        (CU_add_test(my_test_suite3, "any of list is false", test_any_false_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "any of list is true", test_any_true_ll) == NULL) ||
        //apply to all
        (CU_add_test(my_test_suite3, "apply to all of empty list", test_apply_to_all_empty_ll) == NULL) ||
        (CU_add_test(my_test_suite3, "apply to all of non-empty list", test_apply_to_all_non_empty_ll) == NULL) ||
        //// -------------------------- ITERATOR TESTS --------------------------
        //has next
        (CU_add_test(my_test_suite3, "iterator has next false", test_has_next_false_it) == NULL) ||
        (CU_add_test(my_test_suite3, "iterator has next true", test_has_next_true_it) == NULL) ||
        //Next
        (CU_add_test(my_test_suite3, "iterator next of empty list", test_iterate_next_invalid) == NULL) ||
        (CU_add_test(my_test_suite3, "iterator next true", test_iterate_next_possible) == NULL) ||
        //reset
        (CU_add_test(my_test_suite3, "iterator reset of empty list", test_iterate_reset_empty) == NULL) ||
        (CU_add_test(my_test_suite3, "iterator reset of non-empty list", test_iterate_reset_nonempty) == NULL) ||
        //current
        (CU_add_test(my_test_suite3, "iterator current of empty list", test_iterate_current_empty) == NULL) ||
        (CU_add_test(my_test_suite3, "iterator current of non-empty list", test_iterate_current_nonempty) == NULL) ||
        //create destroy
        (CU_add_test(my_test_suite3, "iterator create destroy", test_iterate_create_destroy) == NULL) ||
        0 ) {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);
    //CU_basic_set_mode(CU_BRM_NORMAL);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
} 