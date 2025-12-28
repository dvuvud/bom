#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "../../src/include/refmem.h"

void print_item(item_t *product)
{
    printf("Name:\t%s\n", product -> name);
    printf("Desc:\t%s\n", product -> desc);
    printf("Price:\t%d SEK\n", product -> price / 100);
    printf("Shelf:\t%s\n\n", product -> shelf);
}
// Skap
item_t make_item(char *name, char *desc, int price, char *shelf)
{
    item_t prod = {
        .name = name,
        .desc = desc,
        .price = price,
        .shelf = shelf,
    };
    return prod;
}
// Läs in varor
item_t input_item()
{
    char *name = ask_question_string("Name of product?");
    retain(name); // Spara i databas

    char *desc = ask_question_string("Description?");
    retain(desc);

    int price = ask_question_int("Price (in öre)?");

    char *shelf = ask_question_shelf("Which shelf?");
    retain(shelf);

    return make_item(name, desc, price, shelf);
}
//-----------------------------------------------------
// Database functions
//-----------------------------------------------------

void list_db(item_t *items, int no_items)
{
  for(int i = 0; i < no_items; ++i)
  {
    printf("%d.  %s\n", i + 1, items[i].name);
  }
  printf("\n");
}

void add_item_to_db(item_t *items, int *no_items)
{
  item_t new_item = input_item();
  items[*no_items] = new_item;
  (*no_items)++; //gå till addressen, hämta värdet och öka med ett
  list_db(items, *no_items);
}

void remove_item_from_db(item_t *items, int *no_items)
{
  int num_prod;
  list_db(items, *no_items);
  
  do
    {
      num_prod = ask_question_int("Which item do you want to remove?\n");

    }

    while (num_prod < 1|| num_prod > *no_items);

    item_t *item = &items[num_prod - 1];

    // Frigör minnet för strängarna i varan
    release(item->name);
    release(item->desc);
    release(item->shelf);

    for(int i = num_prod - 1; i < *no_items - 1; ++i)
    {
      items[i] = items[i + 1];
    }
    
    (*no_items)--; // Minska antal varor i databasen
}

void edit_db(item_t *items, int no_items)
{
  int num_prod;
  
  do
  {
    num_prod = ask_question_int("Which item do you want to edit?\n");
  }

  while (num_prod < 1|| num_prod > no_items);
  
  item_t *old = &items[num_prod - 1];

  release(old->name);
  release(old->desc);
  release(old->shelf);

  item_t edited_item = input_item();
  items[num_prod - 1] = edited_item;

  printf("Item has been updated.\n");
}

// Event loop för databasen
void event_loop(item_t *items, int *no_items)
{
    while (true)
    {
        char *action = ask_question_string
        (
            "[L] Add item\n"
            "[T] Remove item\n"
            "[R] Edit item\n"
            "[H] List all\n"
            "[A] Exit\n"
        );

        switch (toupper(action[0]))
        {
            case 'L':
                add_item_to_db(items, no_items);
                break;

            case 'T':
                remove_item_from_db(items, no_items);
                break;

            case 'R':
                edit_db(items, *no_items);
                break;

            case 'H':
                for (int i = 0; i < *no_items; ++i)
                {
                    print_item(&items[i]);
                }
                break;

            case 'A':
                release(action);
                return;
        }

        release(action); 
    }
}

int main(void)
{
    item_t db[16];
    int db_size = 0;

    event_loop(db, &db_size);

    // Frigör minnet för alla varor i databasen innan avslut
    for (int i = 0; i < db_size; ++i)
    {
        release(db[i].name);
        release(db[i].desc);
        release(db[i].shelf);
    }

    shutdown();  // stänger refmem 
    return 0;
}