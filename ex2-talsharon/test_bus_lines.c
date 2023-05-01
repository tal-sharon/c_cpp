#include <stdbool.h>
#include "test_bus_lines.h"

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  for (;start < end; start++)
  {
    if (start->distance > (start+1)->distance)
    {
      return 0;
    }
  }
  return 1;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  for (;start < end; start++)
  {
    if (start->duration > (start+1)->duration)
    {
      return 0;
    }
  }
  return 1;
}

int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original)
{
  bool found_all_lines = true;
  for (BusLine *i = start_original; i <= end_original; i++)
  {
    bool found_line = false;
    for (BusLine *j = start_sorted; j <= end_sorted; j++)
    {
      if (j->line_number == i->line_number)
      {
        found_line = true;
        break;
      }
    }
    if (found_line == false)
    {
      found_all_lines = false;
      break;
    }
  }
  if (found_all_lines == true)
  {
    return 1;
  }
  return 0;
}
