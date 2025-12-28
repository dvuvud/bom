#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

#pragma once

char *copy_string(const char *src);

typedef bool check_func(char *);
typedef union {
    int int_value;
    float float_value;
    char *string_value;
} answer_t;
typedef answer_t convert_func(char *);

void print(char *str);
void println(char *str);
int read_string(char *buf, int buf_siz);
bool is_number(char *str);
int ask_question_int(char *question);
char *ask_question_string(char *question);
answer_t ask_question(char *question, check_func *check, convert_func *convert);

#endif
