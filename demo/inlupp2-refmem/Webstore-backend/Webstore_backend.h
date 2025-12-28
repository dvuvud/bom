#include "../Data-structures/linked_list.h"
#include "../Data-structures/hash_table.h"


#pragma once

/**
 * @file Webstore-backend.h
 * @author Amelie Weiss, Hiba Ahmad
 * @date 12 Nov 2025
 * @brief The backend for a TUI webstore design
 *
 * This file contains the Webstore backend functions and data structures.
 * The warehouse is represented as a hashtable where the key is the merchandise name, and the value is a merchandise struct.
 * To keep track of the locations of the merchandise, another hashtable is used where the key is the shelf name, and the value is the merchandise name.
 * The carts are represented as a hashtable where the key is the cart number, and the value is a list of cart items (merchandise and quantity).
 * 
 * To run these functions, you will need the following files:
 * - linked_list.h
 * - hash_table.h
 * - iterator.h
 * - common.h
 * 
 * To run the frontend you will also need utils.h
 * 
 * You can find the tests for these functions in the file backend-tests.c
 * 
 */


// Definitions of structs
typedef struct merchandise ioopm_merch_t;
typedef struct shelf       ioopm_shelf_t;
typedef struct carts       ioopm_carts_t;
typedef struct cart_item   ioopm_cart_item_t;

struct merchandise
{
    char            *name;          //the name of the merchandise
    char            *description;   //description of merchandise
    int              price;         //price of merchandise
    ioopm_list_t    *locs;          //list of shelves where the merchandise is located
    int              stock;         //total stock of the merchandise
    int              in_cart;       //amount of the merchandise currently in carts
};

struct shelf
{         
    char *shelf;                   //the name of the shelf
    int  quantity;                //the amount of a merchandise on shelf
};

struct carts
{
    ioopm_hash_table_t *carts;     // a hashtable where key: cart number, value: cart (list of cart_items (merch, quantity))
    int                num_carts;  //number of carts created
};



struct cart_item
{
    ioopm_merch_t *merch;       //pointer to the merchandise
    int           quantity;    //quantity of the merchandise in the cart
};

/// @brief create a new merchandise
/// @param name the name of the merchandise
/// @param desc description of merchandise
/// @param price price of merchandise
/// @returns the new created merchandis
ioopm_merch_t *create_merchandise(char *name, char *desc, int price);

/// @brief create a new warehouse hashtable
/// @returns the new created warehouse hashtable
ioopm_hash_table_t *create_warehouse_hash();

/// @brief create a new shelf
/// @param shelf the name of the shelf, that need to be in the format LetterNumberNumber (e.g. A12)
/// @param quantity the amount of a merchandise on shelf, must be greater than 0
/// @returns the new created shelf
ioopm_shelf_t *create_shelf(char *shelf, int quantity);

/// @brief add a new merchandise to the warehouse
/// @param wh the warehouse hashtable
/// @param name the name of the merchandise
/// @param desc description of merchandise
/// @param price price of merchandise
/// @returns the success of the add
bool add_merchandise(ioopm_hash_table_t *wh, char *name, char *desc, int price);

/// @brief returns an array of all merchandise in warehouse in alphabetical order
/// @param wh the warehouse hashtable
/// @returns an array of the names of the merchandise
char **list_merchandise(ioopm_hash_table_t *wh);

/// @brief removes a given merchandise from warehouse, locations and carts
/// @param wh the warehouse hashtable
/// @param locs the locations hashtable
/// @param carts the system of carts
/// @param merch the merchandise to remove
/// @returns boolean showing the success of removal
bool remove_merchandise(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_carts_t *carts, char *merch);

/// @brief edits the merch in a given way
/// @param wh the warehouse hashtable
/// @param carts the system of carts, needed to remove merch from carts if in use
/// @param merch the merchandise to edit
/// @param option one of these options: name: 1, desc: 2, price: 3
/// @param change the change to apply
/// @returns the success of edit
bool edit_merchandise(ioopm_hash_table_t *wh, ioopm_carts_t *carts, char *merch, int option, elem_t change);

/// @brief inserts a shelf into the right alphabetical order in merch stock
/// @param stock The list of stock
/// @param shelf the shelf to insert
void insert_shelf(ioopm_list_t *stock, ioopm_shelf_t *shelf);

/// @brief replenishes the stock of a merch by at least one
/// @param wh the warehouse hashtable
/// @param locs the locations hashtable
/// @param merch the merchandise to replenish
/// @param shelf the location to replenish, if it does not exist it will be created
/// @param increase the amount to replenish, must be greater than 0
/// @returns the success of replenishment
bool replenish_stock(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, char *merch, char *shelf, int increase);

/// @brief creates a new empty system for carts
ioopm_carts_t *create_carts();

/// @brief creates a new empty cart (a linked list) and adds it to the carts hash
/// @param carts the system of created carts
void create_cart(ioopm_carts_t *carts);

/// @brief removes a certain cart
/// @param carts the system of carts
/// @param cart the cart to remove
/// @returns the success of removal
bool remove_cart(ioopm_carts_t *carts, int cart);

/// @brief adds some quantity of a merch to a specific cart
/// @param wh the warehouse hashtable
/// @param carts the system of all previous carts
/// @param cart the cart to add to
/// @param merch the merch to add
/// @param quant the amount to add, must be > 0 and <= available stock
/// @returns if the addition was successful
bool add_to_cart(ioopm_hash_table_t *wh, ioopm_carts_t *carts, int cart, char *merch, int quant);

/// @brief removes some quantity of a merch from a specific cart
/// @param carts the system of all previous carts
/// @param cart the cart to remove from
/// @param merch the merch to remove
/// @param quant the amount to remove, must be > 0 and <= quantity in cart
/// @returns if the removal was successful
bool remove_from_cart(ioopm_carts_t *carts, int cart, char *merch, int quant);

/// @brief calculate the cost of a cart
/// @param carts the system of all previous carts
/// @param cart the cart to calculate
/// @returns the cost of the cart
int calculate_cost(ioopm_carts_t *carts, int cart);

/// @brief checkout a cart
/// @param carts the system of carts
/// @param locs the locations hashtable
/// @param cart the cart to checkout
/// @returns if the checkout was successful
bool checkout_cart(ioopm_carts_t *carts, ioopm_hash_table_t *locs, int cart);

/// @brief quits the program and frees all memory
/// @param wh the warehouse hashtable
/// @param locs the locations hashtable
/// @param carts the system of carts
/// @returns if the quit was successful
bool quit(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_carts_t *carts);
