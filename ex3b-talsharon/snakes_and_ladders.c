#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define ARG_NUM 3
#define SEED_ARG 1
#define MAX_PATHS_ARG 2
#define INPUT_LEN_ERR_MSG "Usage: The program receives 2 arguments only\n"
#define PRINT_MSG "Random Walk %d: "
#define INT_BASE 10
#define REG_PRINT_MSG "[%d] -> "
#define SNAKE_PRINT_MSG "[%d]-snake to %d -> "
#define LADDER_PRINT_MSG "[%d]-ladder to %d -> "
#define LAST_PRINT_MSG "[%d]"
#define LAST_CELL 100


/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
                    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case
                    // there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;


/**
 * Checks if given arguments from command line are valid
 * @param argc number of arguments given from command line
 * @return 0: if all is valid, 1: otherwise
 */
int check_arg_validity (int argc);

/**
 * Initiates the MarkovChain and it's database, allocates needed memory.
 * @return pointer to MarkovChain: upon success, NULL: otherwise
 */
MarkovChain *initiate_database ();

/**
 * Gets the number of paths to be created after the learning process
 * @param argv command line's arguments
 * @return int: the number of paths to be created
 */
int get_paths_num (char *const *argv);

/**
 * Generates random paths by the given number in the command line
 * @param argv command line's arguments
 * @param markov_chain pointer to MarkovChain
 */
void generate_paths (char *argv[], MarkovChain *markov_chain);

/**
 * Gets the random seed given in the command line
 * @param argv
 * @return int: the seed
 */
int get_rand_seed (char *const *argv);

bool is_last_snake (void *data)
{
  Cell *cell = (Cell *) data;
  if (cell->number == LAST_CELL)
  {
    return true;
  }
  return false;
}

int comp_snake (const void *first, const void *second)
{
  Cell *cell1 = (Cell*) first;
  Cell *cell2 = (Cell*) second;
  return cell1->number - cell2->number;
}

void free_snake (void *data)
{
  Cell *cell = (Cell*) data;
  free (cell);
  cell = NULL;
}

void* copy_snake (const void *data)
{
  Cell *cell = (Cell*) data;
  Cell *temp = malloc (sizeof (Cell));
  if (temp == NULL)
  {
    return NULL;
  }
//  memcpy (temp, cell, sizeof (Cell));
  temp->number = cell->number, temp->ladder_to = cell->ladder_to,
  temp->snake_to = cell->snake_to;
  void *ret = (void*) temp;
  return ret;
}

void print_snake (void *data) {
  if (is_last_snake (data))
  {
    Cell *cell = (Cell*) data;
    fprintf (stdout, LAST_PRINT_MSG, cell->number);
    return;
  }
  Cell *cell = (Cell*) data;
  if (cell->ladder_to != EMPTY || cell->snake_to != EMPTY)
  {
    if (cell->ladder_to < cell->snake_to)
    {
      fprintf (stdout, SNAKE_PRINT_MSG, cell->number, cell->snake_to);
    }
    else
    {
      fprintf (stdout, LADDER_PRINT_MSG, cell->number, cell->ladder_to);
    }
  }
  else
  {
    fprintf ( stdout, REG_PRINT_MSG, cell->number);
  }
}



/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
  srand (get_rand_seed (argv));
  if (check_arg_validity (argc) != 0)
  {
    return EXIT_FAILURE;
  }
  MarkovChain *markov_chain = initiate_database ();
  if (markov_chain == NULL)
  {
    return EXIT_FAILURE;
  }
  if (fill_database (markov_chain) == EXIT_FAILURE)
  {
    handle_error (ALLOCATION_ERROR_MASSAGE, &markov_chain);
    return EXIT_FAILURE;
  }
  generate_paths (argv, markov_chain);
  free_markov_chain (&markov_chain);
  return EXIT_SUCCESS;
}

int get_rand_seed (char *const *argv)
{
  char *ptr;
  long seed = strtol (argv[SEED_ARG], &ptr, INT_BASE);
  return (int) seed;
}

int check_arg_validity (int argc)
{
  if (argc != ARG_NUM)
  {
    fprintf (stdout, INPUT_LEN_ERR_MSG);
    return 1;
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
  *markov_chain = (MarkovChain) {database, print_snake, comp_snake,
                                 free_snake, copy_snake, is_last_snake};
//  markov_chain->print_func = print_snake;
//  markov_chain->comp_func = comp_snake;
//  markov_chain->free_data = free_snake;
//  markov_chain->copy_func = copy_snake;
//  markov_chain->is_last = is_last_snake;
  return markov_chain;
}

int get_paths_num (char *const *argv)
{
  char *ptr;
  long paths_num = strtol (argv[MAX_PATHS_ARG], &ptr, INT_BASE);
  return (int) paths_num;
}

void generate_paths (char *argv[], MarkovChain *markov_chain)
{
  for (int n = 0; n < get_paths_num (argv); n++)
  {
    fprintf (stdout, PRINT_MSG, n+1);
    generate_random_sequence (markov_chain,
                              markov_chain->database->first->data,
                              MAX_GENERATION_LENGTH);
    fprintf (stdout, "\n");
  }
}