#include <stdio.h>

int square(int value)
{
    return value * value;
}

bool is_even(int value)
{
    return value % 2 == 0;
}

int main(void)
{
    for (int i = 1; i <= 10; ++i) {
        if (is_even(i)) {
            printf("%d is even, square = %d\n", i, square(i));
        } else {
            printf("%d is odd, square = %d\n", i, square(i));
        }
    }

    return 0;
}