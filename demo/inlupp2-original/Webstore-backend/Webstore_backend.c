#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../Data-structures/linked_list.h"
#include "../Data-structures/hash_table.h"
#include "../Data-structures/iterator.h"
#include "Webstore_backend.h"
#include "../Utils/utils.h"

// -------------------------------------------
//      Helper functions for the backend
// -------------------------------------------

// Hash function for strings
static int string_sum_hash(elem_t e){
  char *str = e.p;
  int result = 0;
  do {
      result += *str;
    } while (*++str != '\0');
  return result;
}

// Comparison function for shelves
static bool compare_shelf(elem_t x, elem_t y)
{
    ioopm_shelf_t *shelf1 = x.p;
    ioopm_shelf_t *shelf2 = y.p;
    if (strcmp(shelf1->shelf, shelf2->shelf) == 0 && shelf1->quantity == shelf2->quantity) return true;
    return false;
}

// Destroy function for shelves
static void destroy_shelf(elem_t not_used, elem_t value, void *extra)
{
    ioopm_shelf_t *shelf = value.p;
    
    free(shelf->shelf);
    free(shelf);
}

// Comparison function for qsort
static int cmpstringp(const void *p1, const void *p2){
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

// Sort an array of strings
static void sort_keys(char *keys[], size_t no_keys){ // From freq-count
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

// Convert a list of strings to an array of strings
static char **List_to_arr(ioopm_list_t *list, size_t list_size)
{
    ioopm_list_iterator_t *iter = ioopm_iterator_create(list);
    size_t no_keys = 0;
    char **arr = calloc(list_size, sizeof(char *)); // create an empty array for pointers to the strings allocated by strdup
    while (no_keys < list_size) {
        if (ioopm_iterator_current(iter).p == NULL) break;
        arr[no_keys] = ioopm_iterator_current(iter).p;
        no_keys += 1;
        ioopm_iterator_next(iter);
    }
    ioopm_iterator_destroy(iter);
    return arr;
}

// Remove all locations of a merchandise from the locations hash table
static void remove_locations(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_merch_t *merch)
{
    ioopm_list_t *locations = merch->locs;
    ioopm_list_iterator_t *locations_iter = ioopm_iterator_create(locations);
    while (ioopm_iterator_current(locations_iter).p) {
        ioopm_shelf_t *current = ioopm_iterator_current(locations_iter).p;
        ioopm_hash_table_remove(locs, ptr_elem(current->shelf));
        
        if (ioopm_iterator_has_next(locations_iter)) {
            ioopm_iterator_next(locations_iter);
        } else {
            break;
        }
    }   
    ioopm_iterator_destroy(locations_iter);
    ioopm_linked_list_apply_to_all(merch->locs, destroy_shelf, NULL);
}

// Find a cart item in a cart by merchandise name
static int find_cart_item(ioopm_list_t *cart, char *merch_name)
{
    ioopm_list_iterator_t *cart_iter = ioopm_iterator_create(cart);
    int index = 0;
    while (ioopm_iterator_current(cart_iter).p)
    {
        elem_t current = ioopm_iterator_current(cart_iter);
        ioopm_cart_item_t *cart_item = current.p;
        if (strcmp(cart_item->merch->name, merch_name) == 0)
        {
            ioopm_iterator_destroy(cart_iter);
            return index;
        }
        if (ioopm_iterator_has_next(cart_iter)) {
            index++;
            ioopm_iterator_next(cart_iter);
        } else break;
    }
    ioopm_iterator_destroy(cart_iter);
    return -1;
}

// Remove a merchandise from all carts
static void remove_from_all_carts(ioopm_carts_t *carts, ioopm_merch_t *merch) {
    ioopm_list_t *all_carts = ioopm_hash_table_values(carts->carts);
    ioopm_list_iterator_t *carts_iter = ioopm_iterator_create(all_carts);
    while (ioopm_iterator_current(carts_iter).p) {
        ioopm_list_t *cart = ioopm_iterator_current(carts_iter).p;
        int cart_index = find_cart_item(cart, merch->name);
        if (cart_index != -1) {
            ioopm_cart_item_t *cart_item = ioopm_linked_list_get(cart, cart_index).p;
            merch->in_cart -= cart_item->quantity;
            ioopm_linked_list_remove(cart, cart_index);
            free(cart_item);
        }
        if (ioopm_iterator_has_next(carts_iter)) {
            ioopm_iterator_next(carts_iter);
        } else {
            break;
    }
    ioopm_iterator_destroy(carts_iter);
    ioopm_linked_list_destroy(all_carts);
    }
}

// Find a shelf by name in a list of shelves
static ioopm_shelf_t *find_shelf(ioopm_list_t *shelves, char *shelf_name)
{
    if (!shelves->head) return NULL;
    ioopm_link_t *cursor = shelves->head->next;
    ioopm_shelf_t *found_shelf = NULL;
    while (cursor)
    {
        found_shelf = cursor->element.p;
        if (str_eq(ptr_elem(found_shelf->shelf), ptr_elem(shelf_name)))
        {
            return found_shelf;
        }
        cursor = cursor->next;
    }
    return NULL;
}

// Comparison function for ordering shelves
static int order_shelf(ioopm_shelf_t *shelf1, ioopm_shelf_t *shelf2)
{
    return strcmp(shelf1->shelf, shelf2->shelf);
}

// Create a new cart item
static ioopm_cart_item_t *create_cart_item(ioopm_merch_t *merch, int quantity)
{
    ioopm_cart_item_t *cart = calloc(1, sizeof(ioopm_cart_item_t));
    cart->merch = merch;
    cart->quantity = quantity;
    return cart;
}

// Destroy a cart item
static void destroy_cart_item(elem_t not_used, elem_t value, void *extra)
{
    ioopm_cart_item_t *cart_item = value.p;
    free(cart_item);
}

// Destroy all carts
static void destroy_all_carts(elem_t not_used, elem_t value, void *extra)
{
    ioopm_list_t *cart = value.p;
    ioopm_linked_list_apply_to_all(cart, destroy_cart_item, NULL);
    ioopm_linked_list_destroy(cart);
}

// Comparison function for merchandise
static bool compare_merch(elem_t a, elem_t b)
{
    ioopm_merch_t *merch1 = a.p;
    ioopm_merch_t *merch2 = b.p;
    if (strcmp(merch1->name, merch2->name) == 0) return true;
    return false;
}

// Remove stock from a merchandise
static void remove_stock(ioopm_merch_t *merch, ioopm_hash_table_t *locs, int quantity)
{
    ioopm_list_t *locations = merch->locs;
    ioopm_list_iterator_t *loc_iter = ioopm_iterator_create(locations);
    int qty_to_remove = quantity;
    while (ioopm_iterator_current(loc_iter).p && qty_to_remove > 0)
    {
        ioopm_shelf_t *shelf = ioopm_iterator_current(loc_iter).p;
        if (shelf->quantity <= qty_to_remove)
        {
            qty_to_remove -= shelf->quantity;
            merch->stock -= shelf->quantity;
            ioopm_shelf_t *next = ioopm_iterator_has_next(loc_iter) ? ioopm_iterator_next(loc_iter).p : NULL;
            ioopm_hash_table_remove(locs, ptr_elem(shelf->shelf));
            destroy_shelf(ptr_elem(NULL), ptr_elem(shelf), NULL);
            ioopm_linked_list_remove(locations, 0);
            if (next) ioopm_iterator_reset(loc_iter);
            else break;
        } else {
            shelf->quantity -= qty_to_remove;
            merch->stock -= qty_to_remove;
            qty_to_remove = 0;
        }
    }
    ioopm_iterator_destroy(loc_iter);
}

// -------------------------------------------
//            Warehouse functions
// -------------------------------------------

ioopm_hash_table_t *create_warehouse_hash()
{
    return ioopm_hash_table_create(string_sum_hash, str_eq);
}

void destroy_warehouse_hash(ioopm_hash_table_t *wh)
{
    ioopm_hash_table_apply_to_all(wh, destroy_merch, NULL);
    ioopm_hash_table_destroy(wh);
}

// -------------------------------------------
//           Merchandise functions
// -------------------------------------------

ioopm_merch_t *create_merchandise(char *name, char *desc, int price)
{
    ioopm_merch_t *new_merch = calloc(1, sizeof(ioopm_merch_t));
    new_merch->name = strdup(name);
    new_merch->description = strdup(desc);
    new_merch->price = price;
    ioopm_list_t *locations = ioopm_linked_list_create(compare_shelf);
    new_merch->locs = locations;
    new_merch->stock = 0;
    new_merch->in_cart = 0;
    return new_merch;
}

void destroy_merch(elem_t not_used, elem_t value, void *extra)
{
    ioopm_merch_t *merch = value.p;
    if (merch) {
        if (merch->locs) 
        {
            ioopm_linked_list_apply_to_all(merch->locs, destroy_shelf, NULL);
            ioopm_linked_list_destroy(merch->locs);
        }
        free(merch->name);
        free(merch->description);
        free(merch);
    }
}

ioopm_shelf_t *create_shelf(char *shelf, int quantity)
{
    ioopm_shelf_t *new_shelf = calloc(1, sizeof(ioopm_shelf_t));
    new_shelf->shelf = strdup(shelf);
    new_shelf->quantity = quantity;
    return new_shelf;
}

bool add_merchandise(ioopm_hash_table_t *wh, char *name, char *desc, int price)
{
    if (!ioopm_hash_table_has_key(wh, ptr_elem(name)))
    {
        ioopm_merch_t *new_merch = create_merchandise(name, desc, price);
        ioopm_hash_table_insert(wh, ptr_elem(new_merch->name), ptr_elem(new_merch));
        return true;
    }
    return false;
}

bool remove_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_carts_t *carts ,char *merch_name)
{
    ioopm_option_t merch_in_wh = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));
    if (merch_in_wh.success) {
        ioopm_merch_t *merch = merch_in_wh.value.p;
        if (merch->in_cart > 0) remove_from_all_carts(carts, merch);
        remove_locations(wh, locs, merch);
        ioopm_list_t *locs_for_merch = merch->locs;
        ioopm_linked_list_destroy(locs_for_merch);
        ioopm_hash_table_remove(wh, ptr_elem(merch_name));
        free(merch->name);
        free(merch->description);
        free(merch);
        return true;
    }
    return false;
}

bool edit_merchandise(ioopm_hash_table_t *wh, ioopm_carts_t *carts, char *merch, int option, elem_t change)
{
    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch));
    if ((!(option > 3) || !(option < 1)) && lookup.success) {
    
        ioopm_merch_t *merch = lookup.value.p;
        if (option == 1) { // Edit name
            if (!change.p) return false;
            char *new_name = change.p;
            if (str_eq(ptr_elem(merch->name), ptr_elem(new_name))) return true;
            if (merch->in_cart > 0) remove_from_all_carts(carts, merch);
            ioopm_hash_table_remove(wh, ptr_elem(merch->name));
            
            free(merch->name);
            merch->name = strdup(new_name);
            ioopm_hash_table_insert(wh, ptr_elem(merch->name), ptr_elem(merch));

        } else if (option == 2) { //Edit description
            if (!change.p) return false;
            char *new_desc = change.p;
            if (merch->in_cart > 0) remove_from_all_carts(carts, merch);
            free(merch->description);
            merch->description = strdup(new_desc);

        } else if (option == 3) { // Edit price
            int new_price = change.i;
            if (merch->in_cart > 0) remove_from_all_carts(carts, merch);
            merch->price = new_price;
        }

        return true;

    } else {
        return false;
    }
}

char **list_merchandise(ioopm_hash_table_t *wh)
{
    ioopm_list_t *all_merch_list = ioopm_hash_table_keys(wh);
    size_t no_keys = ioopm_linked_list_size(all_merch_list);
    if (no_keys == 0)
    {
        ioopm_linked_list_destroy(all_merch_list);
        return NULL;
    }
    char **all_merch = List_to_arr(all_merch_list, no_keys);

    sort_keys(all_merch, no_keys);
    
    ioopm_linked_list_destroy(all_merch_list);
    return all_merch;
}

void insert_shelf(ioopm_list_t *stock, ioopm_shelf_t *shelf)
{
    ioopm_link_t *cursor = stock->head->next;
    size_t stock_size = ioopm_linked_list_size(stock);
    ioopm_shelf_t *cur_shelf = NULL;
    int shelf_index = 0;

    if (stock_size == 0)
    {
        ioopm_linked_list_insert(stock, shelf_index, ptr_elem(shelf));
        return;
    }
    while (cursor && shelf_index <= stock_size && cursor->element.p)
    {   
        shelf_index++;
        cur_shelf = cursor->element.p;
        int comparison = order_shelf(shelf, cur_shelf);
        if (comparison == 0) return;
        if (comparison < 0 || shelf_index == stock_size)
        {
            if (comparison < 0) shelf_index--;
            ioopm_linked_list_insert(stock, shelf_index, ptr_elem(shelf));
            return;
        }
        
        cursor = cursor->next;
    }
}

bool replenish_stock(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, char *merch_name, char *shelf_name, int increase)
{
    if (increase < 1) return false;
    
    ioopm_option_t merch_exists = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));
    ioopm_option_t shelf_exists = ioopm_hash_table_lookup(locs, ptr_elem(shelf_name));

    if (merch_exists.success)
    {
        ioopm_merch_t *merch = merch_exists.value.p;
        if (shelf_exists.success)
        {
            
            if (!str_eq(shelf_exists.value, ptr_elem(merch_name))) return false;
            
            ioopm_shelf_t *shelf = find_shelf(merch->locs, shelf_name);
            if (!shelf) return false;
            shelf->quantity = shelf->quantity + increase;
        } else {
            ioopm_shelf_t *shelf = create_shelf(shelf_name, increase);
            insert_shelf(merch->locs, shelf);
            ioopm_hash_table_insert(locs, ptr_elem(shelf->shelf), ptr_elem(merch->name));
        }
        merch->stock += increase;

        return true;
    }

    return false;
}

// -------------------------------------------
//               Cart functions
// -------------------------------------------

ioopm_carts_t *create_carts()
{
    ioopm_carts_t *carts = calloc(1, sizeof(ioopm_carts_t));
    carts->carts = ioopm_hash_table_create(NULL, NULL);
    carts->num_carts = 0;
    return carts;
}

void destroy_carts(ioopm_carts_t *carts)
{
    ioopm_hash_table_apply_to_all(carts->carts, destroy_all_carts, NULL);
    ioopm_hash_table_destroy(carts->carts);
    free(carts);
}

void create_cart(ioopm_carts_t *carts)
{
    ioopm_list_t *cart = ioopm_linked_list_create(compare_merch);
    ioopm_hash_table_insert(carts->carts, int_elem(carts->num_carts + 1), ptr_elem(cart));
    carts->num_carts++;
}

bool remove_cart(ioopm_carts_t *carts, int cart)
{
    ioopm_option_t cart_lookup = ioopm_hash_table_lookup(carts->carts, int_elem(cart));

    if (cart_lookup.success)
    {
        ioopm_list_t *cart_list = cart_lookup.value.p;
        ioopm_linked_list_apply_to_all(cart_list, destroy_cart_item, NULL);
        ioopm_linked_list_destroy(cart_list);
        ioopm_hash_table_remove(carts->carts, int_elem(cart));
        return true;
    }

    return false;
}

bool add_to_cart(ioopm_hash_table_t *wh, ioopm_carts_t *carts, int cart, char *merch, int quant)
{
    ioopm_option_t cart_lookup = ioopm_hash_table_lookup(carts->carts, int_elem(cart));

    if (cart_lookup.success)
    {
        ioopm_option_t merch_lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch));
        if (merch_lookup.success)
        {
            ioopm_merch_t *ioopm_merch_to_add = merch_lookup.value.p;

            if (ioopm_merch_to_add->stock - ioopm_merch_to_add->in_cart >= quant) {
                
                int cart_index = find_cart_item(cart_lookup.value.p, merch);
                ioopm_cart_item_t *cart_item = NULL;
                ioopm_list_t *cart_list = cart_lookup.value.p;
                if (cart_index == -1)
                {
                    cart_item = create_cart_item(ioopm_merch_to_add, 0);
                    ioopm_linked_list_prepend(cart_list, ptr_elem(cart_item));
                } else {
                    cart_item = ioopm_linked_list_get(cart_list, cart_index).p;
                }
                cart_item->quantity += quant;
                ioopm_merch_to_add->in_cart += quant;
                return true;
            }
        }
    }
    return false;
}

bool remove_from_cart(ioopm_carts_t *carts, int cart, char *merch, int quant)
{
    ioopm_option_t cart_lookup = ioopm_hash_table_lookup(carts->carts, int_elem(cart));

    if (cart_lookup.success)
    {
        ioopm_list_t *cart_list = cart_lookup.value.p;
        int cart_index = find_cart_item(cart_list, merch);

        if (cart_index != -1) 
        {
            ioopm_cart_item_t *cart_item = ioopm_linked_list_get(cart_list, cart_index).p;
            if (cart_item->quantity >= quant)
            {
                cart_item->quantity -= quant;
                cart_item->merch->in_cart -= quant;

                if (cart_item->quantity == 0)
                {
                    free(cart_item);
                    ioopm_linked_list_remove(cart_list, cart_index);
                }

                return true;
            }
        }
        
    }
    return false;
}

int calculate_cost(ioopm_carts_t *carts, int cart)
{
    ioopm_option_t cart_lookup = ioopm_hash_table_lookup(carts->carts, int_elem(cart));
    int total_cost = 0;

    if (cart_lookup.success)
    {
        ioopm_list_t *cart_list = cart_lookup.value.p;
        ioopm_list_iterator_t *cart_iter = ioopm_iterator_create(cart_list);
        while (ioopm_iterator_current(cart_iter).p)
        {
            elem_t current = ioopm_iterator_current(cart_iter);
            ioopm_cart_item_t *cart_item = current.p;
            total_cost += cart_item->merch->price * cart_item->quantity;
            if (ioopm_iterator_has_next(cart_iter)) {
                ioopm_iterator_next(cart_iter);
            } else break;
        }
        ioopm_iterator_destroy(cart_iter);
    }
    return total_cost;
}

bool checkout_cart(ioopm_carts_t *carts, ioopm_hash_table_t *locs, int cart)
{
    ioopm_option_t cart_lookup = ioopm_hash_table_lookup(carts->carts, int_elem(cart));

    if (cart_lookup.success)
    {
        ioopm_list_t *cart_list = cart_lookup.value.p;
        if (ioopm_linked_list_size(cart_list) == 0) {
            ioopm_linked_list_destroy(cart_list);
            ioopm_hash_table_remove(carts->carts, int_elem(cart));
            return true;
        }
        ioopm_list_iterator_t *cart_iter = ioopm_iterator_create(cart_list);
        while (ioopm_iterator_current(cart_iter).p)
        {
            elem_t current = ioopm_iterator_current(cart_iter);
            ioopm_cart_item_t *cart_item = current.p;
            ioopm_merch_t *merch = cart_item->merch;
            
            merch->in_cart -= cart_item->quantity;
            remove_stock(merch, locs, cart_item->quantity);
            free(cart_item);

            if (ioopm_iterator_has_next(cart_iter)) {
                ioopm_iterator_next(cart_iter);
            } else break;

        }
        ioopm_iterator_destroy(cart_iter);
        ioopm_linked_list_destroy(cart_list);
        ioopm_hash_table_remove(carts->carts, int_elem(cart));

        return true;

    }
    return false;
}

// ----- Quit function -----

bool quit(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_carts_t *carts)
{
    destroy_warehouse_hash(wh);
    ioopm_hash_table_destroy(locs);
    destroy_carts(carts);
    return false;
}