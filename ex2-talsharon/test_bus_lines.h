#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

// write only between #define EX2_REPO_TESTBUSLINES_H and
// #endif //EX2_REPO_TESTBUSLINES_H

/**
 * Tests if given BusLine array is sorted by distance.
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * Tests if given BusLine array is sorted by distance.
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * Tests if any BusLine elements from the original array were lost due to
 * sorting action
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);

// write only between #define EX2_REPO_TESTBUSLINES_H and
// #endif //EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
