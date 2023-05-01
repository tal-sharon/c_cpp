#include <stdbool.h>
#include "sort_bus_lines.h"

void bubble_sort (BusLine *start, BusLine *end)
{
  bool swapped = false;
  do
  {
    BusLine *ind = start;
    for (; ind < end; ind++)
    {
      swapped = false;
      BusLine *cur = ind;
      BusLine *next = ind + 1;
      BusLine temp = *cur;
      if (cur->distance > next->distance)
      {
        *ind = *next;
        *(ind + 1) = temp;
        swapped = true;
      }
    }
    if (!swapped)
    {
      break;
    }
  }
  while (1);
}

void quick_sort (BusLine *start, BusLine *end)
{
  if (start != end)
  {
    BusLine *m = partition (start, end);
    if (start != m)
    {
      quick_sort (start, m-1);
    }
    if (end != m)
    {
      quick_sort (m+1, end);
    }
  }
}

BusLine *partition (BusLine *start, BusLine *end)
{
  BusLine *left = start;
  BusLine *right = end;
  BusLine *ind = start - 1;
  for (BusLine *j = left; j < right; j++)
  {
    BusLine temp = *j;
    if (j->duration < right->duration)
    {
      ind++;
      *j = *ind;
      *ind = temp;
    }
  }
  BusLine temp = *right;
  *right = *(ind+1);
  *(ind+1) = temp;
  return (ind+1);
}