#include <stdio.h>

int main(void)
{
    int score = 10;
    int *score_ptr = &score;

    printf("Initial score: %d\n", score);
    printf("Address: %p\n", (void *)score_ptr);
    printf("Value via pointer: %d\n", *score_ptr);

    *score_ptr = 25;

    printf("Updated score: %d\n", score);
    return 0;
}