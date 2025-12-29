#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "../Data-structures/linked_list.h"
#include "../Data-structures/hash_table.h"
#include "../Webstore-backend/Webstore_backend.h"
#include "../Utils/utils.h"

#include <refmem.h>

//----------- Helper functions ------------

///Check if shelf is valid
bool is_shelf(char *shelf) 
{
    if (!shelf || !isalpha((unsigned char)shelf[0]))
    {
        puts("Invalid shelf");
        return false;
    }
    return is_number(shelf + 1);
}

///Ask question that requires shelf input
char *ask_question_shelf(char *question) 
{ 
    return ask_question(question, is_shelf, convert_to_string).string_value;
} 

///Ask a yes/no confirmation question, default answer is given as 'y' or 'n'
bool ask_confirmation(char *question, char default_answer) 
{
    char *answer = ask_question(question, NULL, convert_to_string).string_value;
    retain(answer);
    bool result = (tolower(answer[0]) == default_answer);
    release(answer);
    return result;
}

///Ask question that requires valid cart number
int ask_question_cart(char *question, ioopm_carts_t *carts) 
{
    int cart_num = ask_question_int(question);

    if (cart_num < 1 || cart_num > carts->num_carts || !ioopm_hash_table_has_key(carts->carts, int_elem(cart_num))) 
    {
        puts("Invalid cart number\n");
        return ask_question_cart(question, carts);
    }

    return cart_num;
}


/// Skriver ut huvudmenyn
void print_menu()
{
    puts("\n=============================================");
    puts("     WELCOME TO OUR WEBSTORE MAIN MENU");
    puts("===============================================");
    puts("[A] Add merchandise");
    puts("[L] List all merchandise");
    puts("[D] Delete merchandise");
    puts("[E] Edit merchandise");
    puts("[S] Show stock");
    puts("[P] Replenish stock");
    puts("[C] Create cart");
    puts("[R] Remove cart");
    puts("[+] Add to cart");
    puts("[-] Remove from cart");
    puts("[=] Calculate cart cost");
    puts("[O] Checkout cart");
    puts("[Q] Quit");
    puts("============================");
}

/// Ask for menu option until valid input is given
char *ask_question_menu() 
{
    print_menu();

    char *choice = ask_question_string("Choose an option: ");
    char *menuopt = "AaLlDdEeSsPpCcRr+-=OoQq";

    for (size_t i = 0; i < strlen(menuopt); i++) 
    {
        if (choice[0] == menuopt[i]) 
        {
            return choice;
        }
    }

    puts("Invalid option\n\n");
    return ask_question_menu();
}

/// Generate random merchandise name for testing
char *magick(char **array1, char **array2, char **array3, int size) 
{
    char buf[255];
    snprintf(buf, sizeof(buf), "%s-%s %s",
            array1[rand() % size],
            array2[rand() % size],
            array3[rand() % size]);

    return refmem_strdup(buf);
}

///----------------------------------------------------------------------------------------
///                                 Merchandise actions
///----------------------------------------------------------------------------------------


/// Add merchandise to warehouse
void action_add_merch(ioopm_hash_table_t *wh) 
{
    puts("Insert item details:");
    char *name = ask_question_string("Name: ");
    retain(name);

    // Check if merchandise name already exists
    while (ioopm_hash_table_lookup(wh, ptr_elem(name)).success)
    {
        puts("Name already exists, choose another\n");
        release(name);
        name = ask_question_string("Name: ");
        retain(name);
    }

    char *desc = ask_question_string("Description: ");
    retain(desc);
    int price = ask_question_int("Price (öre): ");

    add_merchandise(wh, name, desc, price);
    puts("Merchandise added\n");

    release(name);
    release(desc);
}


/// List all merchandise in warehouse
void action_list_merch(ioopm_hash_table_t *wh) 
{
    char **all_merch = list_merchandise(wh);
    retain(all_merch);
    int no_merch = ioopm_hash_table_size(wh);
    int x = 20;

    for (int i = 0; i < no_merch; i++) 
    {
        printf("%d. %s\n", i + 1, all_merch[i]);
        x--;
        if (x == 0) 
        {
            if (ask_confirmation("Show more? (y/n): ", 'n')) 
            {
                release(all_merch);
                return;
            } 
            else 
            {
                x = 20;
            }
        }
    }

    printf("\n");
    release(all_merch);
}

/// Edit merchandise in warehouse
void action_edit_merch(ioopm_hash_table_t *wh, ioopm_carts_t *carts) 
{
    char *merch_name = ask_question_string("Choose a merch to edit: ");
    retain(merch_name);

    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));

    // Check if merchandise exists
    if (!lookup.success) 
    {
        puts("Merch not found\n");
        release(merch_name);
        return;
    }

    char *new_name = ask_question_string("New name: ");
    retain(new_name);

    // Check if new name already exists
    if (strcmp(new_name, merch_name) != 0) 
    {
        while (ioopm_hash_table_lookup(wh, ptr_elem(new_name)).success)
        {
            puts("Name already exists, choose another\n");
            release(new_name);
            new_name = ask_question_string("New name: ");
            retain(new_name);
        }
    }

    char *new_desc = ask_question_string("New description: ");
    retain(new_desc);
    int new_price = ask_question_int("New price (öre): ");

    // Edit merchandise after confirmation
    if (ask_confirmation("Save changes? (y/n): ", 'y')) 
    {
        edit_merchandise(wh, carts, merch_name, 1, ptr_elem(new_name));
        edit_merchandise(wh, carts, new_name, 2, ptr_elem(new_desc));
        edit_merchandise(wh, carts, new_name, 3, int_elem(new_price));

        puts("Merch updated\n");
    } 
    else 
    {
        puts("Changes discarded\n");
    }

    release(new_name);
    release(new_desc);
    release(merch_name);
}

/// Delete merchandise from warehouse
void action_delete_merch(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_carts_t *carts, char *merch_name) 
{
    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));

    // Check if merchandise exists
    if (!lookup.success) 
    {
        puts("Merch not found\n");
        return;
    }

    // Confirm deletion
    if (ask_confirmation("Are you sure you want to remove this merch? (y/n): ", 'y')) 
    {
        if (!remove_merchandise(wh, locs, carts, merch_name)) 
        {
            puts("Failed to remove merch\n");
            return;
        }
        puts("Merch removed\n");
    }
}

/// Show stock of a merchandise
void action_show_stock(ioopm_hash_table_t *wh)
{
    char *merch_name = ask_question_string("Choose a merch to show stock: ");
    retain(merch_name);
    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));

    // Check if merchandise exists
    if (!lookup.success) 
    {
        puts("Merch not found\n");
        release(merch_name);
        return;
    }

    ioopm_merch_t *merch = lookup.value.p;
    ioopm_list_t *locations = merch->locs;
    int no_locs = ioopm_linked_list_size(locations);

    // Check if there is stock available
    if (no_locs == 0) 
    {
        puts("No stock available\n");
        release(merch_name);
        return;
    }

    // Print stock information
    printf("Stock for %s:\n", merch_name);
    for (int i = 0; i < no_locs; i++) 
    {
        ioopm_shelf_t *shelf = ioopm_linked_list_get(locations, i).p;
        printf("%s: %d\n", shelf->shelf, shelf->quantity);
    }

    release(merch_name);
}

/// Replenish stock of a merchandise
void action_replenish(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs)
{
    char *shelf_name = ask_question_shelf("Shelf to replenish: ");
    char *merch_name = ask_question_string("Choose a merch to replenish: ");
    retain(shelf_name);
    retain(merch_name);

    // Check if merchandise exists
    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));
    if (!lookup.success) 
    {
        puts("Merch not found\n");
        release(shelf_name);
        release(merch_name);
        return;
    }

    int increase = ask_question_int("Amount to add: ");

    // Validate increase amount
    if (increase < 1) 
    {
        puts("Invalid amount\n");
        return;
    }

    // Replenish stock
    if (replenish_stock(wh, locs, merch_name, shelf_name, increase)) 
    {
        puts("Stock replenished\n");
    } 
    else 
    {
        puts("Failed to replenish stock\n");
    }

    release(shelf_name);
    release(merch_name);
}

///----------------------------------------------------------------------------------------
///                                 Cart actions
///----------------------------------------------------------------------------------------


/// Create a new cart
void action_create_cart(ioopm_carts_t *carts)
{
    create_cart(carts);
    printf("Cart %d created\n", carts->num_carts);
}

/// Remove a cart
void action_remove_cart(ioopm_carts_t *carts)
{
    if (ioopm_hash_table_size(carts->carts) == 0) 
    {
        puts("No carts available\n");
        return;
    }
    int cart_num = ask_question_cart("Choose a cart to remove: ", carts);

    // Confirm removal
    if (ask_confirmation("Are you sure you want to remove this cart? (y/n): ", 'y')) 
    {
        if (remove_cart(carts, cart_num)) 
        {
            puts("Cart removed\n");
        } 
        else 
        {
            puts("Failed to remove cart\n");
        }
    }

}

/// Add merchandise to a cart
void action_add_to_cart(ioopm_carts_t *carts, ioopm_hash_table_t *wh, ioopm_hash_table_t *locs)
{
    if (ioopm_hash_table_size(carts->carts) == 0) 
    {
        puts("No carts available\n");
        return;
    }
    int cart_num = ask_question_cart("Choose a cart to add to:", carts);

    char *merch_name = ask_question_string("Choose a merch to add: ");
    retain(merch_name);
    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));

    // Validate merchandise existence
    if (!lookup.success)
    {
        puts("Merch not found\n");
        release(merch_name);
        return;
    }

    ioopm_merch_t *merch = lookup.value.p;
    int quantity = ask_question_int("Amount to add: ");

    // Validate quantity
    while (quantity < 1 || quantity > (merch->stock - merch->in_cart)) 
    {
        puts("Invalid amount\n");
        release(merch_name);
        return;
    }

    // Add merchandise to cart
    if (add_to_cart(wh, carts, cart_num, merch_name, quantity)) 
    {
        puts("Merch added to cart\n");
    } 
    else 
    {
        puts("Failed to add merch to cart\n");
    }

    release(merch_name);
}

/// Remove merchandise from a cart
void action_remove_from_cart(ioopm_carts_t *carts, ioopm_hash_table_t *wh) 
{
    if (ioopm_hash_table_size(carts->carts) == 0) 
    {
        puts("No carts available\n");
        return;
    }
    int cart_num = ask_question_cart("Choose a cart to remove from:", carts);

    char *merch_name = ask_question_string("Choose a merch to remove: ");
    retain(merch_name);

    // Validate merchandise existence
    ioopm_option_t lookup = ioopm_hash_table_lookup(wh, ptr_elem(merch_name));
    if (!lookup.success) 
    {
        puts("Merch not found\n");
        release(merch_name);
        return;
    }

    ioopm_merch_t *merch = lookup.value.p;
    int quantity = ask_question_int("Amount to remove: ");

    // Validate quantity
    if (quantity == 0) 
    {
        puts("Removed nothing\n");
    }
    if (quantity > merch->in_cart) 
    {
        puts("Invalid amount\n");
        return;
    }

    // Remove merchandise from cart
    if (remove_from_cart(carts, cart_num, merch_name, quantity)) 
    {
        puts("Merch removed from cart\n");
    } 
    else 
    {
        puts("Failed to remove merch from cart\n");
    }

    release(merch_name);
}

/// Calculate total cost of a cart
void action_calculate_cart(ioopm_carts_t *carts) 
{
    if (ioopm_hash_table_size(carts->carts) == 0)
    {
        puts("No carts available\n");
        return;
    }
    int cart_num = ask_question_cart("Choose a cart to calculate cost:", carts);

    // Calculate and display total cost
    int total_cost = calculate_cost(carts, cart_num);
    printf("Total cost of cart (Öre): %d\n", total_cost);
}

/// Checkout a cart
void action_checkout_cart(ioopm_carts_t *carts, ioopm_hash_table_t *locs) 
{
    if (ioopm_hash_table_size(carts->carts) == 0) 
    {
        puts("No carts available\n");
        return;
    }
    int cart_num = ask_question_cart("Choose a cart to checkout:", carts);

    // Checkout cart
    if (checkout_cart(carts, locs, cart_num)) 
    {
        puts("Cart checked out successfully\n");
    } 
    else 
    {
        puts("Failed to checkout cart\n");
    }
}

///-----------------------------------------------------------------------
///                           Event Loop & Main
///-----------------------------------------------------------------------

void event_loop(ioopm_hash_table_t *wh, ioopm_hash_table_t *locs, ioopm_carts_t *carts) 
{
    while (true) 
    {
        char *option = ask_question_menu();
        retain(option);
        char choice = toupper(option[0]);

        if (choice == 'A')
        {
            action_add_merch(wh);

        } else if (choice == 'L') {
            action_list_merch(wh);

        } else if (choice == 'D') {
            char *merch_name = ask_question_string("Choose a merch to remove: ");
            retain(merch_name);
            action_delete_merch(wh, locs, carts, merch_name);
            release(merch_name);
        } else if (choice == 'E') {
            action_edit_merch(wh, carts);

        } else if (choice == 'S') {
            action_show_stock(wh);

        } else if (choice == 'P') {
            action_replenish(wh, locs);

        } else if (choice == 'C') {
            action_create_cart(carts);

        } else if (choice == 'R') {
            action_remove_cart(carts);

        } else if (choice == '+') {
            action_add_to_cart(carts, wh, locs);

        } else if (choice == '-') {
            action_remove_from_cart(carts, wh);

        } else if (choice == '=') {
            action_calculate_cart(carts);

        } else if (choice == 'O') {
            action_checkout_cart(carts, locs);

        } else if (choice == 'Q') {

            if (ask_confirmation("Are you sure you want to quit? (y/n): ", 'y')) 

            {
                release(option);
                break;
            }
        }

        release(option);
    }
} 


int main(int argc, char *argv[]) 
{
    ioopm_hash_table_t *wh = create_warehouse_hash();
    ioopm_hash_table_t *locs = create_warehouse_hash();
    ioopm_carts_t *carts = create_carts();

    event_loop(wh, locs, carts);

    quit(wh, locs, carts);
    shutdown();
    return 0;
}
