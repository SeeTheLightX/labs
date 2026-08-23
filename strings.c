#include <stdio.h>
#include <string.h>

int main(void)
{
    char word[] = "Belize";

    printf("length = %zu\n", strlen(word));

    if (strcmp(word, "Belize") == 0) {
        puts("The strings are equal.");
    }

    return 0;
}