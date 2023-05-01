#include "markov_chain.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * Checks if second node is in first node's counter list.
 * If second node is already in first node's counter list, increases frequency.
 * @param first_node pointer to MarkovNode
                     the node which it's counter list is checked
 * @param second_node pointer to MarkovNode
                      the node which is searched if already in first's list
 * @param markov_chain pointer to MarkovChain
 * @return true: if second node is in first's counter list, false: otherwise
 */
bool is_node_in_counter_list (const MarkovNode *first_node, MarkovNode
*second_node);

/**
* Get random number between 0 and max_number [0, max_number).
* @param max_number maximal number to return (not including)
* @return Random number
*/
int get_random_number(int max_number)
{
  return rand () % max_number;
}

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
  MarkovNode *rand_node = NULL;
  while (1)
  {
    int ind = get_random_number (markov_chain->database->size);
    Node *node_ptr = markov_chain->database->first;
    for (; ind > 0; ind--)
    {
      node_ptr = node_ptr->next;
    }
    if (!markov_chain->is_last (node_ptr->data->data))
    {
      rand_node = node_ptr->data;
      break;
    }
  }
  return rand_node;
}

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  int rand_num = get_random_number
                  (state_struct_ptr->counter_list_full_size);
  int ind = -1;
  while (rand_num >= 0)
  {
    ind++;
    rand_num = rand_num - state_struct_ptr->counter_list[ind]->frequency;
  }
  return state_struct_ptr->counter_list[ind]->markov_node;
}


/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain pointer to MarkovChain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  MarkovNode *cur_node = first_node;
  markov_chain->print_func (cur_node->data);
  for (int i = 0; i < max_length-1; i++)
  {
    cur_node = get_next_random_node (cur_node);
    if (!markov_chain->is_last (cur_node->data))
    {
      markov_chain->print_func (cur_node->data);
    }
    else
    {
      markov_chain->print_func (cur_node->data);
      break;
    }
  }
}

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain(MarkovChain ** ptr_chain)
{
  Node* node_ptr = (*ptr_chain)->database->first;
  while (node_ptr != NULL)
  {
//    NextNodeCounter *counter_list = *node_ptr->data->counter_list;
    for (int i = 0; i < node_ptr->data->counter_list_size; i++)
    {
      free (node_ptr->data->counter_list[i]);
      node_ptr->data->counter_list[i] = NULL;
    }
    free (node_ptr->data->counter_list);
    node_ptr->data->counter_list = NULL;
//    free (node_ptr->data->data);
    (*ptr_chain)->free_data (node_ptr->data->data);
    node_ptr->data->data = NULL;
    free (node_ptr->data);
    node_ptr->data = NULL;
    Node *temp = node_ptr;
    node_ptr = node_ptr->next;
    free (temp);
    temp = NULL;
  }
  free ((*ptr_chain)->database);
  (*ptr_chain)->database = NULL;
  free (*ptr_chain);
  *ptr_chain = NULL;
}

bool
is_node_in_counter_list (const MarkovNode *first_node, MarkovNode
*second_node)
{
  for (int i = 0; first_node->counter_list[i]; i++)
  {
    if (second_node == first_node->counter_list[i]->markov_node)
    {
      first_node->counter_list[i]->frequency++;
      return true;
    }
  }
  return false;
}

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  if (is_node_in_counter_list (first_node, second_node)
      || markov_chain->is_last (first_node->data))
  {
    first_node->counter_list_full_size++;
    return true;
  }
  NextNodeCounter *next_node_counter = malloc (sizeof (NextNodeCounter));
  int counter_list_new_size = first_node->counter_list_size +1;
  NextNodeCounter **temp =
      realloc (first_node->counter_list,
               sizeof (NextNodeCounter) *counter_list_new_size);
  if (next_node_counter == NULL || temp == NULL)
  {
    return false;
  }
  first_node->counter_list = temp;
  next_node_counter->frequency = 1;
  next_node_counter->markov_node = second_node;
  first_node->counter_list[first_node->counter_list_size] = next_node_counter;
  first_node->counter_list[(first_node->counter_list_size)+1] = NULL;
  first_node->counter_list_size++;
  first_node->counter_list_full_size++;
  return true;
}

Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
  Node *node_ptr = markov_chain->database->first;
  while (node_ptr != NULL) //go through all markov chain and look for data
  {
    if (markov_chain->comp_func (node_ptr->data->data, data_ptr) == 0) //found
      // data:return node
    {
      return node_ptr;
    }
    node_ptr = node_ptr->next;
  }
  return node_ptr; //node_ptr should be NULL
}

Node* add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
  Node *node_ptr = get_node_from_database (markov_chain, data_ptr);
  if (node_ptr != NULL)
  {
    return node_ptr;
  }
  MarkovNode *markov_node = malloc (sizeof (MarkovNode));
  NextNodeCounter **counter_list = malloc (sizeof (NextNodeCounter[1]));
//  NextNodeCounter *next_node_counter = malloc (sizeof (NextNodeCounter));
  if (markov_node == NULL || counter_list == NULL ||
      add (markov_chain->database, markov_node) == 1)
  {
    return NULL;
  }
  void *temp = markov_chain->copy_func (data_ptr);
  markov_node->data = temp;
  temp = NULL;
  markov_node->counter_list_size = 0;
  markov_node->counter_list_full_size = 0;
  markov_node->counter_list = counter_list;
  if (markov_chain->is_last (data_ptr))
  {
    *markov_node->counter_list = NULL;
  }
  else
  {
    markov_node->counter_list[0] = NULL;
  }
  return markov_chain->database->last;
}