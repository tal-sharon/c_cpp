#include "markov_chain.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SEED_ARG 1
#define TWEET_NUM_ARG 2
#define INPUT_FILE_ARG 3
#define MAX_FILE_WORDS_ARG 4
#define MIN_ARG_LEN 4
#define MAX_ARG_LEN 5
#define MAX_LINE_LEN 1000
#define INT_BASE 10
#define MAX_TWEET_LENGTH 20
#define INPUT_LEN_ERR_MSG "Usage: The program receives 3 or 4 arguments \
only\n"
#define OPEN_FILE_ERR_MSG "Error: Failed to read the given input file\n"
#define TWEET_MSG "Tweet %d: "

/**
 * Checks if given arguments from command line are valid
 * @param argc number of arguments given from command line
 * @return 0: if all is valid, 1: otherwise
 */
int check_arg_validity (int argc);

/**
 * Opens the given input file from the command line given path
 * @param argv command line's arguments
 * @return pointer to the file: upon success, NULL: otherwise
 */
FILE *get_file (char *const *argv);

/**
 * Initiates the MarkovChain and it's database, allocates needed memory.
 * @return pointer to MarkovChain: upon success, NULL: otherwise
 */
MarkovChain *initiate_database ();

/**
 * Processes a line in the learning stage of the program
 * @param markov_chain pointer to MarkovChain
 * @param tweet pointer to char: a tweet - a line.
 * @param words_to_read int: max number of words to be read from th input
 * @return pointer to the MarkovChain: upon success, NULL: otherwise
 */
MarkovChain *process_single_tweet (MarkovChain *markov_chain, char tweet[],
                                   int words_to_read);

/**
 * Processes a word in the learning stage of the program
 * If the word is not in the given MarkovChain, adds it to the chain's database
 * Adds the word's node to the previous node's counter list
 * @param markov_chain pointer to MarkovChain
 * @param data pointer to char, the data/word to be processed
 * @param prev pointer to Node, the previous node is the MarkovChain
 * @return pointer to the new Node: upon success, NULL: otherwise
 */
Node *process_word (MarkovChain *markov_chain, char data[], Node *prev);

/**
 * Fills the MarkovChain's database - The learning stage of the program.
 * @param fp pointer to FILE: the input file
 * @param words_to_read int: max number of words to be read from the input
 * @param markov_chain pointer to MarkovChain
 * @return 0: upon success, 1: otherwise
 */
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain);

/**
 * Gets the random seed given in the command line
 * @param argv
 * @return int: the seed
 */
int get_seed (char *const *argv);

/**
 * Gets the max number of words given in the command line
 * to read from the input file
 * @param argc number of arguments given from command line
 * @param argv command line's arguments
 * @return int: the max number of words to read from the input file
 */
int get_words_to_read (int argc, char *const *argv);

/**
 * Gets the number of tweets to be created after the learning process
 * @param argv command line's arguments
 * @return int: the number of tweets to be created
 */
int get_tweet_num (char *const *argv);

void generate (char *argv[], MarkovChain *markov_chain);
/**
 * Learns and processes a text corpus, creating a Markov Chain from it's data
 * Generates new sentences randomly based on the Markov Chain
 * @param argc number of arguments given from command line
 * @param argv command line's arguments
 * @return 0: upon success, 1: otherwise
 */
int main (int argc, char *argv[])
{
  srand (get_seed (argv));
  FILE *input_file = get_file (argv);
  if (check_arg_validity (argc) != 0 || input_file == NULL)
  {
    return EXIT_FAILURE;
  }
  MarkovChain *markov_chain = initiate_database ();
  if (markov_chain == NULL)
  {
    fclose (input_file);
    return EXIT_FAILURE;
  }
  int words_to_read = get_words_to_read (argc, argv);
  if (fill_database(input_file, words_to_read, markov_chain) == 1)
  {
    free_markov_chain (&markov_chain);
    fclose (input_file);
    fprintf (stdout , ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  generate (argv, markov_chain);
  free_markov_chain (&markov_chain);
  fclose (input_file);
  return EXIT_SUCCESS;
}

void generate (char *argv[], MarkovChain *markov_chain)
{
  for (int n = 0; n < get_tweet_num (argv); n++)
  {
    fprintf (stdout, TWEET_MSG, n+1);
    generate_random_sequence (markov_chain, NULL, MAX_TWEET_LENGTH);
  }
}

int get_words_to_read (int argc, char *const *argv)
{
  if (argc == MAX_ARG_LEN)
  {
    char *ptr;
    long words_to_read = strtol (argv[MAX_FILE_WORDS_ARG], &ptr,
                             INT_BASE);
    int max_words = (int) words_to_read;
    return max_words;
  }
  return 0;
}

int get_seed (char *const *argv)
{
  char *ptr;
  long seed = strtol (argv[SEED_ARG], &ptr, INT_BASE);
  return (int) seed;
}

int get_tweet_num (char *const *argv)
{
  char *ptr;
  long tweet_num = strtol (argv[TWEET_NUM_ARG], &ptr, INT_BASE);
  return (int) tweet_num;
}

Node *process_word (MarkovChain *markov_chain, char data[], Node *prev)
{
  Node *node = add_to_database(markov_chain, data);
  if (node == NULL)
  {
    return NULL;
  }
  if (prev != NULL)
  {
    if (!add_node_to_counter_list (prev->data, node->data))
    {
      return NULL;
    }
  }
  return node;
}

MarkovChain *process_single_tweet (MarkovChain *markov_chain, char tweet[],
                                   int words_to_read)
{
  const char space[2] = " ";
  char *data = strtok(tweet, space);
  data[strcspn(data, "\n")] = '\0';
  data[strcspn(data, "\r\n")] = '\0';
  Node *prev_node = NULL;
  while (data != NULL &&
  (markov_chain->database->size < words_to_read || words_to_read == 0))
  {
    Node *new_node = process_word (markov_chain, data, prev_node);
    if (new_node == NULL)
    {
      return NULL;
    }
    data = strtok(NULL, space);
    if (data != NULL)
    {
      data[strcspn(data, "\n")] = '\0';
      data[strcspn(data, "\r\n")] = '\0';
    }
    prev_node = new_node;
  }
  return markov_chain;
}

int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  while (words_to_read == 0 || markov_chain->database->size < words_to_read)
  {
    char tweet[MAX_LINE_LEN];
    if (fgets (tweet, MAX_LINE_LEN, fp) == NULL)
    {
      break;
    }
    if (process_single_tweet (markov_chain, tweet, words_to_read) == NULL)
    {
      return 1;
    }
  }
  return 0;
}

MarkovChain *initiate_database ()
{
  LinkedList *database = malloc (sizeof (LinkedList));
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  if (database == NULL || markov_chain == NULL)
  {
    fprintf (stdout , ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  *database = (LinkedList) {NULL, NULL, 0};
  *markov_chain = (MarkovChain) {database};
  return markov_chain;
}

FILE *get_file (char *const *argv)
{
  FILE *input_file = fopen (argv[INPUT_FILE_ARG], "r");
  if (input_file == NULL)
  {
    fprintf (stdout, OPEN_FILE_ERR_MSG);
  }
  return input_file;
}

int check_arg_validity (int argc)
{
  if (argc != MAX_ARG_LEN && argc != MIN_ARG_LEN)
  {
    fprintf (stdout, INPUT_LEN_ERR_MSG);
    return 1;
  }
  return 0;
}
