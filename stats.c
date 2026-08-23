#include "stats.h"

int sum_array(const int *values, size_t count)
{
    int total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += values[i];
    }
    return total;
}