#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

/**
 * @file utils.h
 * @brief Utility functions for input, output, and user interaction.
 */

 /**
 * @brief Structure representing an item in the database.
 *
  */
typedef struct {
    char *name;
    char *desc;
    int price;
    char *shelf;
} item_t;

/**
 * @brief Creates a copy of a string.
 *
 * The returned string is dynamically allocated.
 * The caller is responsible for freeing the memory.
 *
 * @param src The source string to copy
 * @return A newly allocated copy of the string
 */
char *copy_string(const char *src);

/**
 * @brief Type for validation functions.
 *
 * A validation function checks whether a string is valid.
 */
typedef bool check_func(char *);

/**
 * @brief Union used to store different answer types.
 */
typedef union {
    int int_value;        /**< Integer value */
    float float_value;    /**< Floating-point value */
    char *string_value;   /**< String value */
} answer_t;

/**
 * @brief Type for conversion functions.
 *
 * A conversion function converts a string into an answer_t.
 */
typedef answer_t convert_func(char *);

/**
 * @brief Prints a string without a newline.
 *
 * @param str The string to print
 */
void print(char *str);

/**
 * @brief Prints a string followed by a newline.
 *
 * @param str The string to print
 */
void println(char *str);

/**
 * @brief Reads a string from standard input.
 *
 * @param buf Buffer where the input is stored
 * @param buf_siz Size of the buffer
 * @return Number of characters read
 */
int read_string(char *buf, int buf_siz);

/**
 * @brief Checks if a string represents a valid integer number.
 *
 * @param str The string to check
 * @return true if the string is a number, otherwise false
 */
bool is_number(char *str);

/**
 * @brief Converts a string to an integer answer_t.
 *
 * @param str The string to convert
 * @return The converted integer answer_t
 */
answer_t convert_to_int(char *str);

/**
 * @brief Converts a string to a string answer_t.
 *
 * @param str The string to convert
 * @return The converted string answer_t
 */
answer_t convert_to_string(char *str);


/**
 * @brief Asks the user a question and returns an integer answer.
 *
 * @param question The question to ask
 * @return The integer entered by the user
 */
int ask_question_int(char *question);

/**
 * @brief Asks the user a question and returns a string answer.
 *
 * The returned string is dynamically allocated and must be freed
 * by the caller.
 *
 * @param question The question to ask
 * @return A newly allocated string containing the answer
 */
char *ask_question_string(char *question);

/**
 * @brief Generic function for asking a question with validation
 *        and conversion.
 *
 * @param question The question to ask
 * @param check Pointer to a validation function
 * @param convert Pointer to a conversion function
 * @return The converted answer
 */
answer_t ask_question(char *question, check_func *check, convert_func *convert);

#endif
