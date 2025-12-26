#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

//gcc -Wall utils.c db.c


void print_item(item_t *product)
{
    printf("Name:\t%s\n", product -> name);
    printf("Desc:\t%s\n", product -> desc);
    printf("Price:\t%d SEK\n", product -> price / 100);
    printf("Shelf:\t%s\n\n", product -> shelf);
}

item_t make_item(char *name1, char *desc1, int price1, char *shelf1)
{
    item_t prod = { .name = name1, .desc = desc1, .price = price1, .shelf = shelf1};
    print_item(&prod);
    return prod;
}

item_t input_item()
{
    char *name2 = ask_question_string("Name of product?");
    char *desc2 = ask_question_string("Description?");
    int price2 = ask_question_int("Price (in öre)?");
    char *shelf2 = ask_question_shelf("Which shelf?");
  
    return make_item(name2, desc2, price2, shelf2);
}

void list_db(item_t *items, int no_items)
{
  for(int i = 0; i < no_items; ++i)
  {
    printf("%d.  %s\n", i + 1, items[i].name);
  }
  printf("\n");

}

void edit_db(item_t *items, int no_items)
{
  
    int num_prod;
  
    do
    {
        num_prod = ask_question_int("Which item do you want to edit?\n");
        if(num_prod < 1|| num_prod > no_items)
        {
          printf("Invalid index\n");
          
        }
    } while (num_prod < 1|| num_prod > no_items);
    
  
    print_item(&items[num_prod - 1]);
    item_t edited_item = input_item();
    items[num_prod - 1] = edited_item;
    printf("Item has been updated.\n");
    list_db(items, no_items);
    

  }


void print_menu()
{
  char *menu = "[L]ägga till en vara\n"
               "[T]a bort en vara\n"
               "[R]edigera en vara\n"
               "Ån[g]ra senaste ändringen\n"
               "Lista [h]ela varukatalogen\n"
               "[A]vsluta\n\n";

  printf("%s", menu);

}

void add_item_to_db(item_t *items, int *no_items)
{
  item_t new_item = input_item();
  items[*no_items] = new_item;
  *no_items += 1; //gå till addressen, hämta värdet och öka med ett
  list_db(items, *no_items);
  //return new_item;
}

void remove_item_from_db(item_t *items, int *no_items)
{
  int num_prod;
  list_db(items, *no_items);
  
    do
    {
        num_prod = ask_question_int("Which item do you want to remove?\n");
  
        if(num_prod < 1|| num_prod > *no_items)
        {
          printf("Invalid index\n");
          
        }
    } while (num_prod < 1|| num_prod > *no_items);
    
  
    print_item(&items[num_prod - 1]);
    *no_items -= 1;
    printf("Item has been updated.\n");

    for(int i = num_prod - 1; i < *no_items; ++i)
    {
      items[i] = items[i + 1];
    }
    
    list_db(items, *no_items);
    
}

void event_loop(item_t *items, int *no_items) // no_items är en pekare till int, pekaren innehåller addressen till db_siz
{
  //item_t *items;
  //int no_items;

  printf("\n");
 
  char *action = ask_question_menu("[L]ägga till en vara\n"
                                   "[T]a bort en vara\n"
                                   "[R]edigera en vara\n"
                                   "Ån[g]ra senaste ändringen\n"
                                   "Lista [h]ela varukatalogen\n"
                                   "[A]vsluta\n\n");

  printf("Your choice: %s\n", action);


  if(action[0] == 'L')
  {
    add_item_to_db(items, no_items);
    event_loop(items, no_items);
  }
  else if(action[0] == 'T')
  {
    remove_item_from_db(items, no_items); //skicka pekaren
    event_loop(items, no_items);
  }
  else if(action[0] == 'R')
  {
    edit_db(items, *no_items);
    event_loop(items, no_items);

  }
  else if(action[0] == 'G')
  {
    printf("Not yet implemented!\n");
    event_loop(items, no_items);
  }
  else if(action[0] == 'H')
  {
    for (int i = 0; i < *no_items; ++i)
     {
       print_item(&items[i]);
     }

    event_loop(items, no_items);
  }
  else if(action[0] == 'A')
  {
     return;
  }

  
}

int main(int argc, char *argv[])
{
  char *array1[] = {"Laser", "Polka", "Extra" }; // TODO: Lägg till!
  char *array2[] = { "förnicklad", "smakande", "ordinär" }; // TODO: Lägg till!
  char *array3[] = { "skruvdragare", "kola", "uppgift" }; // TODO: Lägg till!

  int array_len = sizeof(array1) / sizeof(array1[0]);

  if (argc != 1)
  {
    printf("Usage: %s", argv[0]);
  }
  else
  {
    item_t db[16]; // Array med plats för 16 varor
    int db_siz = 0; // Antalet varor i arrayen just nu
/*
    int items = atoi(argv[1]); // Antalet varor som skall skapas

    if (items > 0 && items <= 16)
    {
      for (int i = 0; i < items; ++i)
      {
        // Läs in en vara, lägg till den i arrayen, öka storleksräknaren
        item_t item = input_item();
        db[db_siz] = item;
        ++db_siz;
      }
    }
    else
    {
     // puts("Sorry, must have [1-16] items in database.");
      return 1; // Avslutar programmet!
    }*/

    for (int i = db_siz; i < 16; ++i)
      {
        char *name = magick(array1, array2, array3, array_len); // TODO: Lägg till storlek
        char *desc = magick(array1, array2, array3, array_len); // TODO: Lägg till storlek
        int price = random() % 200000;
        char shelf[] = { random() % ('Z'-'A') + 'A',
                         random() % 10 + '0',
                         random() % 10 + '0',
                         '\0' };
        item_t item = make_item(name, desc, price, strdup(shelf));

        db[db_siz] = item;
        ++db_siz;
      }

     // Skriv ut innehållet
    event_loop(db, &db_siz); //adressen där värdet på db_siz finns
    
     //list_db(db, db_siz);
     //edit_db(db, db_siz);
/*
     for (int i = 0; i < db_siz; ++i)
     {
       print_item(&db[i]);
     }
  */
  }

 
  return 0;
}