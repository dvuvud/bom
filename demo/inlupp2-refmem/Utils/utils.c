#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <refmem.h>

#include "utils.h"


// String helper function (replacement for strdup)
char *copy_string(const char *src)
{
  if (!src)
  {
    return NULL;
  }

  size_t len = strlen(src) + 1;
  char *copy = allocate_array(len, sizeof(char), NULL);
  memcpy(copy, src, len);
  return copy;
}


bool is_number(char *str)
{
    if (!str || *str == '\0') 
    {
      return false;
    }

    for (size_t i = 0; i < strlen(str); i++)
    {
        if (!isdigit((unsigned char)str[i]) && !(i == 0 && str[i] == '-'))
        {
            return false;
        }
    }
    return true;
}

void print(char *str)
{
  if (!str)
  {
    return;
  }
  
  for (size_t i = 0; i < strlen(str); i++)
    {
        putchar(str[i]);
    }
}

void println(char *str)
{
   print(str);
   putchar('\n');
}

static void clear_input_buffer(void) 
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

int read_string(char *buf, int buf_siz)
{
    int c;
    int i = 0;

    while (i < buf_siz - 1 && (c = getchar()) != '\n' && c != EOF)
    {
        buf[i++] = (char)c;
    }

    buf[i] = '\0';

    if (c != '\n')
    {
        clear_input_buffer();
    }

    return 0;
}

static bool not_empty(char *str)
{
  return str && strlen(str) > 0;
}

answer_t convert_to_int(char *str)
{
    answer_t answer;
    answer.int_value = atoi(str);
    return answer;
}
 answer_t convert_to_string(char *str)
{
    answer_t answer;
    answer.string_value = copy_string(str);
    return answer;
}

answer_t ask_question(char *question, check_func *check, convert_func *convert)
{
  char buf[255];

  while (true)
  {
    println(question);
    read_string(buf, sizeof(buf));

    if (!check || check(buf))
    {
      return convert(buf);
    }
  }
}

int ask_question_int(char *question)
{
  return ask_question(question, is_number, convert_to_int).int_value;
 
}

char *ask_question_string(char *question)
{
  return ask_question(question, not_empty, convert_to_string).string_value;
}
