#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#define ARG_LEN 2
#define COMMAND_ARG 1
#define MAX_LINE_LEN 60
#define INT_BASE 10
#define INPUT_UPPER_BOUND 1000
#define INPUT_LOWER_BOUND 0
#define DURATION_LOWER_BOUND 10
#define DURATION_UPPER_BOUND 100


/**
 * Checks if the given command is valid.
 * @param argc
 * @param argv
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE otherwise.
 */
int command_validity (int argc, char *const *argv);

/**
 * Gets the number of lines from the user.
 * @return An integer equals to the number of lines.
 */
long get_num_of_lines ();

/**
 * Checks the information's validity of each line.
 * @return EXIT_SUCCESS upon success, EXIT_FAILURE otherwise.
 */
int line_info_validity (char *line_info, BusLine *bus_line);

/**
 * Gets all lines information from the user and creates a dynamic array of
 * BusLine struct containing the given information.
 * @param num_of_lines An integer equals to the number of lines.
 * @return A dynamic BusLine struct containing all lines information.
 */
BusLine *get_lines_info (long num_of_lines);

/**
 * Sorts the BusLine array by the given command - bubble or quick.
 * @param num_of_lines An integer equals to the number of lines.
 * @param bus_lines A pointer to a dynamic array of BusLine.
 * @param command A string of sort type.
 */
void
run_sort (long num_of_lines, BusLine *bus_lines, const char *command);

/**
 * Runs tests on both sorts - bubble and quick.
 * @param num_of_lines An integer equals to the number of lines.
 * @param bus_lines A pointer to a dynamic array of BusLine.
 * @param copy A pointer to a copy of the dynamic array of BusLine.
 */
void run_tests (long num_of_lines, BusLine *bus_lines, BusLine *copy);

/**
 * Runs the tests of bubble sort.
 * @param num_of_lines An integer equals to the number of lines.
 * @param bus_lines A pointer to a dynamic array of BusLine.
 * @param start_sorted A pointer to the the start of the sorted array.
 * @param end_sorted A pointer to the the end of the sorted array.
 * @param start_original A pointer to the the start of the original array.
 * @param end_original A pointer to the the end of the original array.
 */
void test_bubble_sort (long num_of_lines, BusLine *bus_lines,
                       BusLine *start_sorted, BusLine *end_sorted,
                       BusLine *start_original, BusLine *end_original);

/**
 *
 * Runs the tests of quick sort.
 * @param num_of_lines An integer equals to the number of lines.
 * @param bus_lines A pointer to a dynamic array of BusLine.
 * @param start_sorted A pointer to the the start of the sorted array.
 * @param end_sorted A pointer to the the end of the sorted array.
 * @param start_original A pointer to the the start of the original array.
 * @param end_original A pointer to the the end of the original array.
 */
void test_quick_sort (long num_of_lines, BusLine *bus_lines,
                      BusLine *start_sorted, BusLine *end_sorted,
                      BusLine *start_original, BusLine *end_original);

/**
 *
 * @param num_of_lines An integer equals to the number of lines.
 * @param bus_lines A pointer to a dynamic array of BusLine.
 * @param copy A pointer to a copy of the dynamic array of BusLine.
 * @param command A string of the given command by the user.
 */
void run_command (long num_of_lines, BusLine *bus_lines,
                  BusLine *copy, const char *command);

/**
 * Frees memory allocated earlier in the program and sets pointer to NULL
 * @param dynamic_array A pointer to the memory to free
 */
void free_memory (BusLine *dynamic_array);

/**
 * Main function of the program.
 * Sorts a given set of Bus lines by distance or duration by request.
 * Checks validity of given command and acts accordingly.
 */
int main (int argc, char *argv[])
{
  if (command_validity (argc, argv) != 0) // check if arguments are valid
  {
    return EXIT_FAILURE;
  }
  long num_of_lines = get_num_of_lines (); // get number of lines from user
  BusLine *bus_lines = get_lines_info (num_of_lines); // get all lines info
  if (bus_lines == NULL) // check if memory allocation was successful
  {
    return EXIT_FAILURE;
  }
  BusLine *copy = calloc (num_of_lines, sizeof (BusLine));
  if (copy == NULL) // check if memory allocation was successful
  {
    free_memory (bus_lines); // before exit free existing allocated memory
    return EXIT_FAILURE;
  }
  memcpy (copy, bus_lines, num_of_lines * sizeof (BusLine));
  char *command = argv[COMMAND_ARG];
  run_command (num_of_lines, bus_lines, copy, command);
  free_memory (bus_lines);
  free_memory (copy);
  return EXIT_SUCCESS;
}

void free_memory (BusLine *dynamic_array)
{
  free (dynamic_array);
  dynamic_array = NULL;
}

void run_command (long num_of_lines, BusLine *bus_lines,
                  BusLine *copy, const char *command)
{
  if (strcmp (command, "test") == 0)
  {
    run_tests (num_of_lines, bus_lines, copy);
  }
  else
  {
    run_sort (num_of_lines, bus_lines, command);
    for (int i = 0; i < num_of_lines; i++)
    {
      fprintf (stdout, "%d,%d,%d\n", bus_lines[i].line_number,
               bus_lines[i].distance, bus_lines[i].duration);
    }
  }
}

void run_tests (long num_of_lines, BusLine *bus_lines, BusLine *copy)
{
  BusLine *start_sorted = &bus_lines[0];
  BusLine *end_sorted = &bus_lines[num_of_lines-1];
  BusLine *start_original = &copy[0];
  BusLine *end_original = &copy[num_of_lines-1];
  test_bubble_sort (num_of_lines, bus_lines, start_sorted, end_sorted,
                    start_original, end_original);
  test_quick_sort (num_of_lines, bus_lines, start_sorted, end_sorted,
                   start_original, end_original);
}

void test_quick_sort (long num_of_lines, BusLine *bus_lines,
                      BusLine *start_sorted, BusLine *end_sorted,
                      BusLine *start_original, BusLine *end_original)
{
  run_sort (num_of_lines, bus_lines, "quick");
  if (is_sorted_by_duration (start_sorted, end_sorted) == 0)
  {
    fprintf (stdout,"TEST 3 FAILED: testing the array is sorted by "
                    "duration\n");
  }
  else
  {
    fprintf (stdout, "TEST 3 PASSED: testing the array is sorted by "
                    "duration\n");
  }
  if (is_equal (start_sorted, end_sorted, start_original, end_original) ==
  0)
  {
    fprintf (stdout, "TEST 4 FAILED: testing the array have the same "
            "items after sorting\n");
  }
  else
  {
    fprintf (stdout, "TEST 4 PASSED: testing the array have the same "
            "items after sorting\n");
  }
}

void test_bubble_sort (long num_of_lines, BusLine *bus_lines,
                       BusLine *start_sorted, BusLine *end_sorted,
                       BusLine *start_original, BusLine *end_original)
{
  run_sort (num_of_lines, bus_lines, "bubble");
  if (is_sorted_by_distance (start_sorted, end_sorted) == 0)
  {
    fprintf (stdout, "TEST 1 FAILED: testing the array is sorted by "
                    "distance\n");
  }
  else
  {
    fprintf (stdout, "TEST 1 PASSED: testing the array is sorted by "
                    "distance\n");
  }
  if (is_equal (start_sorted, end_sorted, start_original, end_original) ==
  0)
  {
    fprintf (stdout, "TEST 2 FAILED: testing the array have the same "
            "items after sorting\n");
  }
  else
  {
    fprintf (stdout, "TEST 2 PASSED: testing the array have the same "
            "items after sorting\n");
  }
}

void
run_sort (long num_of_lines, BusLine *bus_lines, const char *command)
{
  BusLine *start = &bus_lines[0];
  BusLine *end = &bus_lines[num_of_lines-1];
  if (strcmp (command, "bubble") == 0)
  {
    bubble_sort (start, end);
  }
  if (strcmp (command, "quick") == 0)
  {
    quick_sort(start, end);
  }
}

BusLine *get_lines_info (long num_of_lines)
{
  BusLine *bus_lines = malloc (sizeof (BusLine) * num_of_lines);
  if (bus_lines == NULL)
  {
    return NULL;
  }
  int i = 0;
  while (i < num_of_lines)
  {
    char line_info[MAX_LINE_LEN];
    fprintf (stdout, "Enter line info. Then enter\n");
    fgets (line_info, MAX_LINE_LEN, stdin);
    if (line_info_validity (line_info, &bus_lines[i]) == 0)
    {
      i++;
    }
  }
  return bus_lines;
}

int line_info_validity (char *line_info, BusLine *bus_line)
{
  char *remain = "\0";
  long line_num = strtol (line_info, &remain, INT_BASE);
  if (line_num <= INPUT_LOWER_BOUND || line_num >= INPUT_UPPER_BOUND)
  {
    fprintf (stdout, "ERROR: Line number should be an integer between 1 "
                     "and 999 (includes)\n");
    return EXIT_FAILURE;
  }
  line_info = remain + 1; // move pointer to the next number
  long distance = strtol (line_info, &remain, INT_BASE);
  if (distance < INPUT_LOWER_BOUND || distance > INPUT_UPPER_BOUND)
  {
    fprintf (stdout, "ERROR: distance should be an integer between 0 "
                     "and 1000 (includes)\n");
    return EXIT_FAILURE;
  }
  line_info = remain + 1; // move pointer to the next number
  long duration = strtol (line_info, &remain, INT_BASE);
  if (duration < DURATION_LOWER_BOUND || duration > DURATION_UPPER_BOUND)
  {
    fprintf (stdout, "ERROR: duration should be an integer between 10 "
                     "and 100 (includes)\n");
    return EXIT_FAILURE;
  }
  bus_line->line_number = line_num;
  bus_line->distance = distance;
  bus_line->duration = duration;
  return EXIT_SUCCESS;
}

long get_num_of_lines ()
{
  long num_of_lines = 0;
  long *ptr = &num_of_lines;
  do
  {
    char input[MAX_LINE_LEN];
    char *endptr = "\0";
    fprintf (stdout, "Enter number of lines. Then enter\n");
    fgets (input, MAX_LINE_LEN, stdin);
    *ptr = strtol (input, &endptr, INT_BASE);
    int is_valid = true;
    for (int i = 0; input[i] != '\n'; i++)
    {
      if ((isdigit (input[i]) == 0) || (input[i] == '0'))
      {
        fprintf (stdout, "ERROR: Number of input entered isn't a positive "
                         "integer\n");
        is_valid = false;
        break;
      }
    }
    if (is_valid)
    {
      break;
    }
  } while(1);
  return num_of_lines;
}

int command_validity (int argc, char *const *argv)
{
  if (argc != ARG_LEN)
  {
    fprintf (stdout, "USAGE: The program only takes 1 argument, %d was given"
                     ".\n", argc-1);
    return EXIT_FAILURE;
  }
  if ((strcmp (argv[COMMAND_ARG], "bubble") != 0)
  && (strcmp (argv[COMMAND_ARG], "quick") != 0)
  && (strcmp (argv[COMMAND_ARG], "test") != 0))
  {
    fprintf (stdout, "USAGE: Invalid command.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
