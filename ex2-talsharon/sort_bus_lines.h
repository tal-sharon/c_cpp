#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and
// #endif //EX2_REPO_SORTBUSLINES_H

/**
 * A struct of a Bus Line
 * Has a bus number and the distance and duration of the bus drive.
 */
typedef struct BusLine
{
    int line_number, distance, duration;
} BusLine;

/**
 * Sorts the BusLine elements by distance using Bubble-Sort algorithm.
 */
void bubble_sort (BusLine *ind, BusLine *end);

/**
 * Sorts the BusLine elements by duration using Quick-Sort algorithm.
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * Main sorting algorithm code of the Quick-Sort.
 * Sorting the BusLine elements by duration.
 */
BusLine *partition (BusLine *start, BusLine *end);

// write only between #define EX2_REPO_SORTBUSLINES_H and
// #endif //EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
