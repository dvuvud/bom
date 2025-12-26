#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hash_table.h"
#include "linked_list.h"
#include "refmem.h"

//Default hash function
int hash_function(elem_t key){
    return key.i % No_Buckets;
}

//Static functions
static entry_t *find_previous_entry_for_key(entry_t *bucket, elem_t new_key, ioopm_eq_function *eq_fun) {
    //returns the entry before the one we are looking for,
    //or, in case such an entry does not exist, the entry
    //whose next pointer should be pointing to the entry once
    //we have inserted it.
    entry_t *cursor = bucket;
    while(cursor->next) {
        if (eq_fun(cursor->next->key, new_key)) {
            break;
        }
        cursor = cursor->next;
    }
    return cursor;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next){
    entry_t *new_entry = allocate(sizeof(entry_t), NULL);

    if (key.p != NULL) {
        retain(key.p);
    }
    if (value.p != NULL) {
        retain(value.p);
    }

    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = next;
    return new_entry;
}

static void entry_destroy(entry_t *entry, ioopm_eq_function *eq_fun) {
    if (entry->key.p != NULL) {
        release(entry->key.p);
    }
    if (entry->value.p != NULL) {
        release(entry->value.p);
    }
    deallocate(entry);
}

static bool value_equiv(elem_t key_ignored, elem_t value, void *x){
    char *other_value = x;
    return strcmp(value.p, other_value)==0;
}

//END of static functions

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hf, ioopm_eq_function *eq_fun) {
    //create an empty hash table with dummy entries in each bucket
    ioopm_hash_table_t *result = allocate(sizeof(ioopm_hash_table_t), NULL);
    for (int i = 0; i < No_Buckets; i++){
        result->buckets[i] = entry_create(int_elem(0),ptr_elem(NULL),NULL);
    }
    if (hf == NULL){
        result->hash_func = hash_function;
    } else {
        result->hash_func = hf;
    }
    if (eq_fun == NULL){
        result->eq_fun = int_eq;
    } else {
        result->eq_fun = eq_fun;
    }

    return result;
}


void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) {
    entry_t *next;
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]; //Hoppa över dummy
        while(current != NULL) { //För varje entry i given bucket (som inte är tom) - frigör
            next = current->next; //spara nästa entry
            entry_destroy(current, ht->eq_fun); //radera innehållet
            current = next; // gå vidare till nästa entry inom bucketen
        }
    }
    release(ht);
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) {
    /// Calculate the bucket for this entry
    ioopm_hash_function *hf = ht->hash_func;
    int bucket = abs(hf(key))%No_Buckets;
    /// Search for an existing entry for a key
    entry_t *current = ht->buckets[bucket]; //skip dummy
    entry_t *entry = find_previous_entry_for_key(current, key, ht->eq_fun);
    entry_t *next = entry->next;

    /// Check if the next entry should be updated or not
    if (next != NULL && ht->eq_fun(next->key, key)) {
        if (next->value.p != NULL) {
            release(next->value.p);
        }
        if (value.p != NULL) {
            retain(value.p);
        }
        next->value = value;
    }
    else {
        entry->next = entry_create(key, value, next);
    }
}

ioopm_option_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key){
    ioopm_hash_function *hf = ht->hash_func;
    /// Find the previous entry for key
    entry_t *tmp = find_previous_entry_for_key(ht->buckets[abs(hf(key))%No_Buckets], key, ht->eq_fun);
    entry_t *next = tmp->next;

    if (next) {
        /// If entry was found, return the adress for that value.
        ioopm_option_t res = { .success = true, .value = next->value};
        return res;
    }
    else {
        ioopm_option_t res = {.success = false};
        return res;
    }
}

ioopm_option_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) {
    ioopm_hash_function *hf = ht->hash_func;
    //start by finding the element before the one we want removed
    entry_t *tmp = find_previous_entry_for_key(ht->buckets[abs(hf(key))%No_Buckets], key, ht->eq_fun);
    if (!(tmp->next) || !(ht->eq_fun(tmp->next->key, key))){ // if the element cannot be found
        ioopm_option_t res = {.success = false}; //return false
        return res;
    } else { //The key exists
        entry_t *entry_to_remove = tmp->next;
        elem_t removed_value = entry_to_remove->value;
        tmp->next = entry_to_remove->next;
        entry_destroy(entry_to_remove, ht->eq_fun);
        ioopm_option_t res = {.success = true, .value = removed_value};
        return res;
    }

}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht){
    size_t count = 0;
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]->next; // hoppa över dummy
        while(current) { //För varje entry i given bucket (som inte är tom) öka räknaren
            count++;
            current = current->next; // gå vidare till nästa entry inom bucketen
        }
    }
    return count;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht){
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]->next; // hoppa över dummy
        while(current) { //Om den hittar en entry
            return false;
        }
    }
    return true;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht){
    entry_t *next;
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]->next; //Skip dummy
        while(current) { //För varje entry i given bucket (som inte är tom) - frigör
            next = current->next; //spara nästa entry
            entry_destroy(current, ht->eq_fun); //radera innehållet
            current = next; // gå vidare till nästa entry inom bucketen
        }
        ht->buckets[i]->next = NULL; //Återställ dummy-nod
    }
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht){
    ioopm_list_t *res = ioopm_linked_list_create(ht->eq_fun); //skapa en linked list som keysen ska lagras i
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]->next; //Skip dummy
        while(current) { //För varje entry i given bucket (som inte är tom)
            ioopm_linked_list_append(res,current->key);
            current = current->next; // gå vidare till nästa entry inom bucketen
        }
    }
    return res;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht){
    ioopm_list_t *res = ioopm_linked_list_create(ht->eq_fun); //skapa en linked list
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){//Loopa igenom alla buckets och lägg till varje entrys value
        current = ht->buckets[i]->next; //Skip dummy
        while(current) { //För varje entry i given bucket (som inte är tom)
            ioopm_linked_list_append(res,current->value);
            current = current->next; // gå vidare till nästa entry inom bucketen
        }
    }
    return res;
}



bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg){
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]->next; // hoppa över dummy
        while(current) { //Om den hittar en entry
            if (!pred(current->key, current->value, arg)){ //Om villkoret är falskt - returnera falskt
                return false;
            }
            current = current->next;
        }
    }
    return true;
}


bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate *pred, void *arg){
    entry_t *current;
    for (int i = 0; i < No_Buckets; i++){
        current = ht->buckets[i]->next; // hoppa över dummy
        while(current) { //Om den hittar en entry
            if (pred(current->key, current->value, arg)){ //Om predikatet är sant för något
                return true;
            }
            current = current->next;
        }
    }
    return false;
}


bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key){
    ioopm_hash_function *hf = ht->hash_func;
    int bucket = abs(hf(key)) % No_Buckets;

    entry_t *cur = ht->buckets[bucket]->next;
    while (cur) {
        if (ht->eq_fun(cur->key, key)) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function *apply_fun, void *arg){
    entry_t *current;

    size_t size = ioopm_hash_table_size(ht);
    if (size == 0) return;

    for (int i = 0; i < No_Buckets; i++){ //Loopa igenom samtliga buckets och deras länkade listor
        current = ht->buckets[i]->next; // hoppa över dummy
        while(current) { //Om den hittar en entry - applicera funktionen på den
            apply_fun(current->key, current->value, arg);
            current = current->next;
        }
    }
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value){
    return ioopm_hash_table_any(ht, value_equiv, value.p);
}