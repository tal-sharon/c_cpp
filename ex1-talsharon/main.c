#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COMMAND_LENGTH 5
#define COMMAND_TEST_LEN 2
#define COMMAND_ARG 1
#define SHIFT_VAL_ARG 2
#define INPUT_ARG 3
#define OUTPUT_ARG 4
#define MAX_LINE_LEN 1024
#define NUM_BASE 10

/**
 * Runs all the tests from tests.h
 * @return EXIT_SUCCESS when passes all tests, otherwise EXIT_FAILURE
 */
int run_tests ();

/**
 * Checks the validation of the CLI arguments (when gets 4 arguments)
 * @param command
 * @param shift_val
 * @param remain the string remaining from the shift value argument
 * @param input_file
 * @param output_file
 * @return EXIT_SUCCESS when all are valid, otherwise EXIT_FAILURE
 */
int check_validity_cipher (char *command, int shift_val, char *remain, FILE
                          *input_file, FILE *output_file);

/**
 * Runs the cipher action suitable to the given arguments
 * @param command
 * @param shift_val
 * @param input_file
 * @param output_file
 */
void
run_cipher (char *command, int shift_val, FILE *input_file, FILE *output_file);

/**
 * When getting 4 arguments, runs validation checks.
 * If all is valid, runs the cipher.
 * @param argv
 * @return EXIT_SUCCESS upon success, otherwise EXIT_FAILURE
 */
int cipher_command (char *const *argv);


int main (int argc, char *argv[])
{
  if (argc == COMMAND_LENGTH) // if got 4 arguments
  {
    return cipher_command (argv);
  }
  else if (argc == COMMAND_TEST_LEN) // if got 1 arguments
  {
    if (strcmp (argv[COMMAND_ARG], "test") != 0)
    {
      fprintf (stderr, "Usage: cipher test\n");
      return EXIT_FAILURE;
    }
    return run_tests ();
  }
  else // if number of arguments is not 4 or 1
  {
    fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
    return EXIT_FAILURE;
  }
}

int cipher_command (char *const *argv)
{
  char remain[1] = "";
  char *ptr = remain;
  int shift_val = strtol (argv[SHIFT_VAL_ARG], &ptr, NUM_BASE);
  FILE *input_file = fopen (argv[INPUT_ARG], "r");
  FILE *output_file = fopen (argv[OUTPUT_ARG], "w");
  if (check_validity_cipher (argv[COMMAND_ARG], shift_val, ptr, input_file,
                             output_file) != 0)
  {
    return EXIT_FAILURE;
  }
  run_cipher (argv[COMMAND_ARG], shift_val, input_file, output_file);
  return EXIT_SUCCESS;
}

void
run_cipher (char *command, int shift_val, FILE *input_file, FILE *output_file)
// check command type and act accordingly
{
  char msg[MAX_LINE_LEN] = "\0";
  if (strcmp (command, "encode") == 0)
  {
    while (fgets (msg, MAX_LINE_LEN, input_file) != NULL)
    {
      encode (msg, shift_val);
      fprintf (output_file, "%s", msg);
    }
  }
  if (strcmp (command, "decode") == 0)
  {
    while (fgets (msg, MAX_LINE_LEN, input_file) != NULL)
    {
      decode (msg, shift_val);
      fprintf (output_file, "%s", msg);
    }
  }
}

int check_validity_cipher (char *command, int shift_val, char *remain, FILE
                          *input_file, FILE *output_file)
{
  if ((strcmp (command, "encode") != 0 && (strcmp (command, "decode") != 0)))
  {
    fprintf (stderr, "The given command is invalid.\n");
    return EXIT_FAILURE;
  }
  // check if shift value is valid
  if ((shift_val == 0) || (strcmp (remain, "\0") != 0))
  {
    fprintf (stderr, "The given shift value is invalid.\n");
    return EXIT_FAILURE;
  }
  // check if input file is valid
  if (input_file == NULL)
  {
    fprintf (stderr, "The given file is invalid.\n");
    return EXIT_FAILURE;
  }
  // check if output file is valid
  if (output_file == NULL)
  {
    fclose (input_file);
    fprintf (stderr, "The given file is invalid.\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int run_tests ()
{
  // test encode
  if ((test_encode_non_cyclic_lower_case_positive_k () != 0)
      || (test_encode_cyclic_lower_case_special_char_positive_k () != 0)
      || (test_encode_non_cyclic_lower_case_special_char_negative_k () != 0)
      || (test_encode_cyclic_lower_case_negative_k () != 0)
      || (test_encode_cyclic_upper_case_positive_k () != 0))
  {
    return EXIT_FAILURE;
  }
  // test decode
  if ((test_decode_non_cyclic_lower_case_positive_k () != 0)
      || (test_decode_cyclic_lower_case_special_char_positive_k () != 0)
      || (test_decode_non_cyclic_lower_case_special_char_negative_k () != 0)
      || (test_decode_cyclic_lower_case_negative_k () != 0)
      || (test_decode_cyclic_upper_case_positive_k () != 0))
  {
    return EXIT_FAILURE;
  }
  // all tests passed successfully
  return EXIT_SUCCESS;
}
