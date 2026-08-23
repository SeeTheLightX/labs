#include <stdio.h>
#include <stdlib.h>
#include "stats.h"
#include "record.h"
#include "node.h"

int main(void)
{
    // Part 3 & 6: Array & sum_array
    int numbers[] = {10, 20, 30, 40};
    size_t count = sizeof numbers / sizeof numbers[0];
    printf("Array Sum: %d\n", sum_array(numbers, count));

    // Part 7: Stack Record & Pointer
    Record stack_rec = {.id = 101, .value = 99.5};
    Record *rec_ptr = &stack_rec;
    rec_ptr->value = 100.0;
    printf("Stack Record ID: %d, Value: %.1f\n", rec_ptr->id, rec_ptr->value);

    // Part 8: Dynamic Record
    Record *heap_rec = malloc(sizeof *heap_rec);
    if (heap_rec == NULL) {
        fputs("Heap record allocation failed\n", stderr);
        return EXIT_FAILURE;
    }
    heap_rec->id = 202;
    heap_rec->value = 42.5;
    printf("Heap Record ID: %d, Value: %.1f\n", heap_rec->id, heap_rec->value);
    free(heap_rec);

    // Part 9: Dynamic Node
    Node *node = malloc(sizeof *node);
    if (node == NULL) {
        fputs("Node allocation failed\n", stderr);
        return EXIT_FAILURE;
    }
    node->value = 100;
    node->next = NULL;
    printf("Node Value: %d\n", node->value);
    free(node);

    return EXIT_SUCCESS;
}