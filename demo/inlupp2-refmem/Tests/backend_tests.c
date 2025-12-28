#include <CUnit/Basic.h>
#include <stdlib.h>

#include "../Webstore-backend/Webstore_backend.h"
#include "../Data-structures/linked_list.h"
#include "../Data-structures/hash_table.h"
#include "refmem.h"

int init_suite(void)  { return 0; }

int clean_suite(void) { return 0; }

void test_create_merch()
{
    ioopm_merch_t *new_merch = create_merchandise("redbull", "energydrink", 20);
    CU_ASSERT_TRUE(str_eq(ptr_elem(new_merch->name), ptr_elem("redbull")));
    CU_ASSERT_TRUE(str_eq(ptr_elem(new_merch->description), ptr_elem("energydrink")));
    CU_ASSERT_EQUAL(new_merch->price, 20);
    deallocate(new_merch);
}

void test_create_shelf()
{
    ioopm_shelf_t *new_shelf = create_shelf("C11", 10);
    CU_ASSERT_TRUE(str_eq(ptr_elem(new_shelf->shelf), ptr_elem("C11")));
    CU_ASSERT_EQUAL(new_shelf->quantity, 10);
    deallocate(new_shelf);
}

void test_empty_warehouse()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    CU_ASSERT_PTR_NOT_NULL(warehouse);
    deallocate(warehouse);
}

void test_add_one_merch()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    char *name = "Redbull";
    char *desc = "Energydrink";
    CU_ASSERT_TRUE(add_merchandise(warehouse, name, desc, 20));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(warehouse, ptr_elem(name)));
    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;
    CU_ASSERT_EQUAL(merch->price, 20);
    CU_ASSERT_TRUE(str_eq(ptr_elem(merch->description), ptr_elem(desc)));
    deallocate(warehouse);
}

void test_add_multiple_merch()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    char *name2 = "Kexchoklad";
    char *desc2 = "Choklad";
    add_merchandise(warehouse, name2, desc2, 10);
    char *name3 = "Banan";
    char *desc3 = "frukt";
    add_merchandise(warehouse, name3, desc3, 15);

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(warehouse, ptr_elem(name)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(warehouse, ptr_elem(name2)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(warehouse, ptr_elem(name3)));

    deallocate(warehouse);
}

void test_add_same_merch()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    char *name = "Redbull";
    char *desc = "Energydrink";
    CU_ASSERT_TRUE(add_merchandise(warehouse, name, desc, 20));
    CU_ASSERT_FALSE(add_merchandise(warehouse, name, desc, 20));
    deallocate(warehouse);
}

void test_list_empty_warehouse()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    char **all_merch = list_merchandise(warehouse);
    CU_ASSERT_PTR_NULL(all_merch);
    deallocate(warehouse);
    deallocate(all_merch);
}

void test_list_one_merch()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    char **all_merch = list_merchandise(warehouse);
    CU_ASSERT_TRUE(str_eq(ptr_elem(name), ptr_elem(all_merch[0])));

    deallocate(all_merch);
    deallocate(warehouse);
}

void test_list_multiple_merch()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20); //TODO: fixa enklare sätt att bygga större warehouse
    char *name2 = "Kexchoklad";
    char *desc2 = "Choklad";
    add_merchandise(warehouse, name2, desc2, 10);
    char *name3 = "Banan";
    char *desc3 = "frukt";
    add_merchandise(warehouse, name3, desc3, 15);

    char **all_merch = list_merchandise(warehouse);
    CU_ASSERT_TRUE(str_eq(ptr_elem(name3), ptr_elem(all_merch[0])));
    CU_ASSERT_TRUE(str_eq(ptr_elem(name2), ptr_elem(all_merch[1])));
    CU_ASSERT_TRUE(str_eq(ptr_elem(name), ptr_elem(all_merch[2])));

    deallocate(all_merch);
    deallocate(warehouse);
}

void test_remove_empty_warehouse()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();
    char *name = "Redbull";
    CU_ASSERT_FALSE(remove_merchandise(warehouse, locs, carts, name));
    deallocate(warehouse);
    deallocate(locs);
    deallocate(carts);
}

void test_remove_merch_once()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();
    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    //ADD LOCATIONS
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(warehouse, ptr_elem(name)));
    CU_ASSERT_TRUE(remove_merchandise(warehouse, locs, carts, name));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(warehouse, ptr_elem(name)));
    deallocate(warehouse);
    deallocate(locs);
    deallocate(carts);
}

void test_remove_multiple()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    char *name2 = "Kexchoklad";
    char *desc2 = "Choklad";
    add_merchandise(warehouse, name2, desc2, 10);
    char *name3 = "Banan";
    char *desc3 = "frukt";
    add_merchandise(warehouse, name3, desc3, 15);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(warehouse), 3);

    remove_merchandise(warehouse, locs, carts, name);
    remove_merchandise(warehouse, locs, carts, name2);
    remove_merchandise(warehouse, locs, carts, name3);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(warehouse), 0);

    deallocate(locs);
    deallocate(warehouse);
    deallocate(carts);
}

void test_remove_non_existent()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    char *name2 = "Kexchoklad";
    char *desc2 = "Choklad";
    add_merchandise(warehouse, name2, desc2, 10);

    char *not_added = "Delicatoboll";
    CU_ASSERT_FALSE(remove_merchandise(warehouse, locs, carts, not_added));

    deallocate(warehouse);
    deallocate(locs);
    deallocate(carts);
}

void test_edit_name()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    char *new_name = "Monster";
    CU_ASSERT_TRUE(edit_merchandise(warehouse, carts, name, 1, ptr_elem(new_name)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(warehouse, ptr_elem(new_name)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(warehouse, ptr_elem(name)));

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(new_name)).value.p;
    CU_ASSERT_TRUE(str_eq(ptr_elem(merch->name), ptr_elem(new_name)));
    CU_ASSERT_EQUAL(merch->price, 20);

    deallocate(warehouse);
    deallocate(carts);

}

void test_edit_all()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();

    char *name = "Redbull";
    char *desc = "Energydrink";
    int new_price = 15;
    char* new_name = "Cola-Zero";
    add_merchandise(warehouse, name, desc, 20);
    edit_merchandise(warehouse, carts, name, 1, ptr_elem(new_name));
    edit_merchandise(warehouse, carts, new_name, 2, ptr_elem("Läsk"));
    edit_merchandise(warehouse, carts, new_name, 3, int_elem(new_price));
    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(new_name)).value.p;

    CU_ASSERT_TRUE(str_eq(ptr_elem(merch->description), ptr_elem("Läsk")));
    CU_ASSERT_TRUE(str_eq(ptr_elem(merch->name), ptr_elem(new_name)));
    CU_ASSERT_EQUAL(merch->price, new_price);

    deallocate(warehouse);
    deallocate(carts);
}

void test_edit_non_existent()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();

    char *name = "Redbull";
    CU_ASSERT_FALSE(edit_merchandise(warehouse, carts, name, 1, ptr_elem("Monster")));

    deallocate(warehouse);
    deallocate(carts);
}

void test_replenish_once()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 2);

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;

    ioopm_shelf_t *shelf = ioopm_linked_list_get(merch->locs, 0).p;


    CU_ASSERT_EQUAL(shelf->quantity, 2); //Check if location is saved in merch
    CU_ASSERT_TRUE(strcmp(shelf->shelf, "A34") == 0); //Check if locations is stored in hash

    replenish_stock(warehouse, locs, name, "A34", 3);

    CU_ASSERT_EQUAL(shelf->quantity, 5); //Check if quantity is updated

    deallocate(warehouse);
    deallocate(locs);
}

void test_replenish_multiple()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";

    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 2);
    replenish_stock(warehouse, locs, name, "B78", 7);

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;

    ioopm_shelf_t *shelf1 = ioopm_linked_list_get(merch->locs, 0).p;
    ioopm_shelf_t *shelf2 = ioopm_linked_list_get(merch->locs, 1).p;

    CU_ASSERT_TRUE(strcmp(shelf1->shelf, "A34") == 0); //Check if location is saved in merch
    CU_ASSERT_TRUE(strcmp(shelf2->shelf, "B78") == 0);

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(locs, ptr_elem("A34"))); //Check if locations is stored in hash
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(locs, ptr_elem("B78")));

    deallocate(warehouse);
    deallocate(locs);
}

void test_replenish_same_shelf()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";

    char *name2 = "Cola-zero";
    char *desc2 = "Läsk";

    add_merchandise(warehouse, name, desc, 20);
    add_merchandise(warehouse, name2, desc2, 15);

    CU_ASSERT_TRUE(replenish_stock(warehouse, locs, name, "A34", 2));
    CU_ASSERT_FALSE(replenish_stock(warehouse, locs, name2, "A34", 16)); //Check that we can only use shelf for one type of merch

    deallocate(warehouse);
    deallocate(locs);
}

void test_replenish_invalid_incr()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";

    add_merchandise(warehouse, name, desc, 20);

    CU_ASSERT_FALSE(replenish_stock(warehouse, locs, name, "A34", 0));
    CU_ASSERT_FALSE(replenish_stock(warehouse, locs, name, "A34", -2));

    deallocate(warehouse);
    deallocate(locs);
}

void test_insert_shelf_once()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";

    add_merchandise(warehouse, name, desc, 20);

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;
    ioopm_shelf_t *shelf = create_shelf("A4", 5);
    insert_shelf(merch->locs, shelf);
    CU_ASSERT_TRUE(ioopm_linked_list_contains(merch->locs, ptr_elem(shelf)));

    deallocate(warehouse);
    deallocate(locs);
}

void test_insert_shelf_multiple()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";

    add_merchandise(warehouse, name, desc, 20);

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;
    ioopm_shelf_t *shelf = create_shelf("A4", 5);
    retain(shelf);
    ioopm_shelf_t *shelf2 = create_shelf("B5", 1);
    retain(shelf);
    ioopm_shelf_t *shelf3 = create_shelf("A1", 3);
    retain(shelf);

    char **shelf_arr = calloc(3, sizeof(ioopm_shelf_t *));
    shelf_arr[0] = "A1";
    shelf_arr[1] = "A4";
    shelf_arr[2] = "B5";

    insert_shelf(merch->locs, shelf);
    insert_shelf(merch->locs, shelf2);
    insert_shelf(merch->locs, shelf3);

    int i = 0;
    while (i < 3)
    {
        ioopm_shelf_t *current = ioopm_linked_list_get(merch->locs, i).p;
        CU_ASSERT_TRUE(strcmp(current->shelf, shelf_arr[i]) == 0);
        i++;
    }

    deallocate(shelf_arr);
    deallocate(warehouse);
    deallocate(locs);
}

void test_create_destroy_cart()
{
    ioopm_carts_t *carts = create_carts();
    create_cart(carts);
    CU_ASSERT_PTR_NOT_NULL(ioopm_hash_table_lookup(carts->carts, int_elem(1)).value.p);
    CU_ASSERT_TRUE(carts->num_carts == 1);

    deallocate(carts);
}

void test_remove_cart()
{
    ioopm_carts_t *carts = create_carts();
    create_cart(carts);
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));
    CU_ASSERT_TRUE(remove_cart(carts, 1));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));

    deallocate(carts);
}

void test_remove_non_existent_cart()
{
    ioopm_carts_t *carts = create_carts();

    CU_ASSERT_FALSE(remove_cart(carts, 1));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));

    deallocate(carts);
}

void test_remove_multiple_carts()
{
    ioopm_carts_t *carts = create_carts();
    create_cart(carts);
    create_cart(carts);
    create_cart(carts);
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(carts->carts, int_elem(2)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(carts->carts, int_elem(3)));

    CU_ASSERT_TRUE(remove_cart(carts, 1));
    CU_ASSERT_TRUE(remove_cart(carts, 2));
    CU_ASSERT_TRUE(remove_cart(carts, 3));

    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(2)));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(3)));

    create_cart(carts);

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(carts->carts, int_elem(4)));

    deallocate(carts);
}

void test_add_to_cart_non_existent_cart()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();



    CU_ASSERT_FALSE(add_to_cart(warehouse, carts, 1, "Redbull", 2));

    deallocate(carts);
    deallocate(warehouse);
}

void test_add_to_cart_once()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 10);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    ioopm_list_t *cart = ioopm_hash_table_lookup(carts->carts, int_elem(1)).value.p;

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 2));
    ioopm_cart_item_t *cart_merch = ioopm_linked_list_get(cart, 0).p;
    char *merch_name = cart_merch->merch->name;
    int merch_quant = cart_merch->quantity;
    CU_ASSERT_TRUE(strcmp(merch_name, name) == 0);
    CU_ASSERT_TRUE(merch_quant == 2);

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_add_to_cart_multiple()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();
    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    char *name2 = "Kexchoklad";
    char *desc2 = "Choklad";
    add_merchandise(warehouse, name2, desc2, 10);
    char *name3 = "Banan";
    char *desc3 = "frukt";
    add_merchandise(warehouse, name3, desc3, 15);

    replenish_stock(warehouse, locs, name, "A34", 10);
    replenish_stock(warehouse, locs, name2, "B12", 5);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);
    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 2));
    CU_ASSERT_FALSE(add_to_cart(warehouse, carts, 1, "Kexchoklad", 6)); //Not enough stock
    CU_ASSERT_FALSE(add_to_cart(warehouse, carts, 1, "Banan", 1)); //No stock

    CU_ASSERT_FALSE(add_to_cart(warehouse, carts, 2, "Redbull", 9)); //Stock in another cart

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_add_to_cart_same_merch()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 10);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 2));
    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 3));

    ioopm_list_t *cart = ioopm_hash_table_lookup(carts->carts, int_elem(1)).value.p;
    ioopm_cart_item_t *cart_merch = ioopm_linked_list_get(cart, 0).p;

    CU_ASSERT_EQUAL(cart_merch->quantity, 5);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(cart), 1);

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_remove_from_cart_non_existent()
{
    ioopm_carts_t *carts = create_carts();

    CU_ASSERT_FALSE(remove_from_cart(carts, 1, "Redbull", 2));

    deallocate(carts);
}

void test_remove_from_cart_once()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 10);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 2));
    CU_ASSERT_TRUE(remove_from_cart(carts, 1, "Redbull", 2));

    ioopm_list_t *cart = ioopm_hash_table_lookup(carts->carts, int_elem(1)).value.p;
    CU_ASSERT_EQUAL(ioopm_linked_list_size(cart), 0);

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_remove_from_cart_multiple()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 10);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);
    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 10));

    CU_ASSERT_FALSE(add_to_cart(warehouse, carts, 2, "Redbull", 1)); //No stock left

    CU_ASSERT_TRUE(remove_from_cart(carts, 1, "Redbull", 2));

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 2, "Redbull", 1)); //Stock freed up

    ioopm_list_t *cart = ioopm_hash_table_lookup(carts->carts, int_elem(1)).value.p;
    ioopm_cart_item_t *cart_merch = ioopm_linked_list_get(cart, 0).p;

    CU_ASSERT_EQUAL(cart_merch->quantity, 8);

    CU_ASSERT_TRUE(remove_from_cart(carts, 1, "Redbull", 8));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(cart), 0);

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_calculate_empty_cart()
{
    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    int total_cost = calculate_cost(carts, 1);
    CU_ASSERT_EQUAL(total_cost, 0);

    deallocate(carts);
}

void test_calculate_cart_one_item()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 10);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 3));

    int total_cost = calculate_cost(carts, 1);
    CU_ASSERT_EQUAL(total_cost, 60);

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_calculate_multiple_items()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    char *name2 = "Kexchoklad";
    char *desc2 = "Choklad";
    add_merchandise(warehouse, name2, desc2, 10);

    replenish_stock(warehouse, locs, name, "A34", 10);
    replenish_stock(warehouse, locs, name2, "B12", 5);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 2));
    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Kexchoklad", 3));

    int total_cost = calculate_cost(carts, 1);
    CU_ASSERT_EQUAL(total_cost, 70);

    deallocate(carts);
    deallocate(warehouse);
    deallocate(locs);
}

void test_checkout_non_existent_cart()
{
    ioopm_carts_t *carts = create_carts();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    CU_ASSERT_FALSE(checkout_cart(carts, locs, 1));

    deallocate(carts);
    deallocate(locs);
}

void test_checkout_empty_cart()
{
    ioopm_carts_t *carts = create_carts();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    create_cart(carts);

    CU_ASSERT_TRUE(checkout_cart(carts, locs, 1));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));

    deallocate(carts);
    deallocate(locs);
}

void test_checkout_one_cart()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 10);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 2));

    CU_ASSERT_TRUE(checkout_cart(carts, locs, 1));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;
    ioopm_shelf_t *shelf = ioopm_linked_list_get(merch->locs, 0).p;
    CU_ASSERT_EQUAL(shelf->quantity, 8); // Check that stock is reduced

    quit(warehouse, locs, carts);
}

void test_checkout_multiple_shelves()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);

    replenish_stock(warehouse, locs, name, "A34", 5);
    replenish_stock(warehouse, locs, name, "B12", 7);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 10));

    CU_ASSERT_TRUE(checkout_cart(carts, locs, 1));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));

    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;

    CU_ASSERT_EQUAL(ioopm_linked_list_size(merch->locs), 1); //Check that one shelf is gone
    ioopm_shelf_t *remaining_shelf = ioopm_linked_list_get(merch->locs, 0).p;
    CU_ASSERT_EQUAL(remaining_shelf->quantity, 2); //Check that quantity is correct

    quit(warehouse, locs, carts);
}

void test_checkout_all_stock()
{
    ioopm_hash_table_t *warehouse = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();

    char *name = "Redbull";
    char *desc = "Energydrink";
    add_merchandise(warehouse, name, desc, 20);
    ioopm_merch_t *merch = ioopm_hash_table_lookup(warehouse, ptr_elem(name)).value.p;

    replenish_stock(warehouse, locs, name, "A34", 5);
    replenish_stock(warehouse, locs, name, "B12", 7);

    ioopm_carts_t *carts = create_carts();

    create_cart(carts);

    CU_ASSERT_TRUE(add_to_cart(warehouse, carts, 1, "Redbull", 12));
    CU_ASSERT_EQUAL(merch->in_cart, 12);
    CU_ASSERT_EQUAL(merch->stock, 12);

    CU_ASSERT_TRUE(checkout_cart(carts, locs, 1));
    CU_ASSERT_FALSE(ioopm_hash_table_has_key(carts->carts, int_elem(1)));


    CU_ASSERT_EQUAL(ioopm_linked_list_size(merch->locs), 0); // Check that all stock is gone
    CU_ASSERT_EQUAL(merch->stock, 0);
    CU_ASSERT_EQUAL(merch->in_cart, 0);

    quit(warehouse, locs, carts);
}



int main() {
  if (CUE_SUCCESS != CU_initialize_registry())
  {
    return CU_get_error();
  }
    CU_pSuite suite = CU_add_suite("Backend Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    // ALL test functions here:
    CU_add_test(suite, "Create merchandise", test_create_merch);
    CU_add_test(suite, "Create shelf", test_create_shelf);
    CU_add_test(suite, "Create empty warehouse", test_empty_warehouse);

    CU_add_test(suite, "Add merch to warehouse once", test_add_one_merch);
    CU_add_test(suite, "Add multiple merch to warehouse", test_add_multiple_merch);
    CU_add_test(suite, "Add same merch twice", test_add_same_merch);

    CU_add_test(suite, "list merch on empty warehouse", test_list_empty_warehouse);
    CU_add_test(suite, "list one merch", test_list_one_merch);
    CU_add_test(suite, "list multiple merch", test_list_multiple_merch);

    CU_add_test(suite, "Remove on empty warehouse", test_remove_empty_warehouse);
    CU_add_test(suite, "Remove once", test_remove_merch_once);
    CU_add_test(suite, "Remove multiple", test_remove_multiple);
    CU_add_test(suite, "Remove non-existent merch", test_remove_non_existent);

    CU_add_test(suite, "Edit name for merch", test_edit_name);
    CU_add_test(suite, "Edit name, description and price", test_edit_all);
    CU_add_test(suite, "Edit non-existent merch", test_edit_non_existent);

    CU_add_test(suite, "Replenish merch twice with one shelf", test_replenish_once);
    CU_add_test(suite, "Replenish merch with two shelves", test_replenish_multiple);
    CU_add_test(suite, "Replenish with same shelf for two merch", test_replenish_same_shelf);
    CU_add_test(suite, "Replenish with invalid increase", test_replenish_invalid_incr);

    CU_add_test(suite, "Insert shelf once", test_insert_shelf_once);
    CU_add_test(suite, "Insert shelf multiple", test_insert_shelf_multiple);

    CU_add_test(suite, "Create carts system and insert one empty cart", test_create_destroy_cart);

    CU_add_test(suite, "Remove empty cart", test_remove_cart);
    CU_add_test(suite,"Remove non-existent", test_remove_non_existent_cart);
    CU_add_test(suite, "Remove empty cart", test_remove_multiple_carts);

    CU_add_test(suite, "Add to non-existent cart", test_add_to_cart_non_existent_cart);
    CU_add_test(suite, "Add to cart once", test_add_to_cart_once);
    CU_add_test(suite, "Add to cart multiple", test_add_to_cart_multiple);
    CU_add_test(suite, "Add same merch to cart", test_add_to_cart_same_merch);

    CU_add_test(suite, "Remove from non-existent cart", test_remove_from_cart_non_existent) ;
    CU_add_test(suite, "Remove from cart once", test_remove_from_cart_once);
    CU_add_test(suite, "Remove from cart multiple", test_remove_from_cart_multiple);

    CU_add_test(suite, "Calculate cost of empty cart", test_calculate_empty_cart);
    CU_add_test(suite, "Calculate cost of cart with one item", test_calculate_cart_one_item);
    CU_add_test(suite, "Calculate cost of cart with multiple items", test_calculate_multiple_items);

    CU_add_test(suite, "Checkout non-existent cart", test_checkout_non_existent_cart);
    CU_add_test(suite, "Checkout empty cart", test_checkout_empty_cart);
    CU_add_test(suite, "Checkout one cart", test_checkout_one_cart);
    CU_add_test(suite, "Checkout cart with merch with multiple shelves", test_checkout_multiple_shelves) ;
    CU_add_test(suite, "Checkout cart that buys all stock", test_checkout_all_stock);

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  shutdown();
  return CU_get_error();
}
