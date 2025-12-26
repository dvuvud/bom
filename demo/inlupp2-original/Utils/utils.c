#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

extern char *strdup(const char *);

// typedef bool check_func(char *);
// typedef union {
//     int int_value;
//     float float_value;
//     char *string_value;
// } answer_t;
// typedef answer_t convert_func(char *);

bool is_number(char *str) 
{
   for (int i = 0; i < strlen(str) ; i++) 
  {
    char currentString = str[i];
    bool isCurrentNumber = isdigit(currentString);
    char minus = '-'; 
    if (isCurrentNumber == false && !(i == 0 && currentString == minus )) {
      return false;
    }
  }
  return true;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

void print(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        putchar(str[i]);
    }
}

void println(char *str)
{
   print(str);
   print("\n");
}

int clear_input_buffer() 
{
  int c;
  do 
    {
      c = getchar();
    }
  while (c != '\n' && c != EOF);
  return 0;
}

int read_string(char *buf, int buf_siz)
{
  int c;
  int counter = 0;
  do
    {
     c = getchar();
     if (c != '\n')
     {
      buf[counter] = c;
      counter++;
     }
    }
  while (counter < buf_siz - 1 && c != '\n' && c != EOF);
  buf[counter] = '\0';
  if (counter == buf_siz - 1)
  {
    clear_input_buffer();
  }
  return 0;

}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
    int buf_siz = 255;
    char buf[buf_siz];
    while (true)
    {
        println(question);
        read_string(buf, buf_siz);
        if (check == NULL)
        {
            break;
        }
        if (check(buf))
        {
            break;
        }
    }
    return convert(buf);
}

int ask_question_int(char *question)
{
  answer_t answer = ask_question(question, is_number, (convert_func *) atoi);
  return answer.int_value;
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, (convert_func *) strdup).string_value;
}