#include <stdio.h>
#include <stddef.h>

int sum_array(const int *values, size_t count)
{
    int total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += values[i];
    }
    return total;
}

int main(void)
{
    int values[] = {4, 7, 1, 9, 3, 12, 5}; // Updated to 7 elements
    size_t count = sizeof values / sizeof values[0];

    printf("Count: %zu\nElements: ", count);
    for (size_t i = 0; i < count; ++i) {
        printf("%d ", values[i]);
    }
    printf("\nSum: %d\n", sum_array(values, count));

    return 0;
}