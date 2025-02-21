#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /path/to/input\r\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\r\n", argv[0]);
        return 1;
    }

    uint64_t* pebbles = malloc(100 * sizeof(uint64_t));
    if (!pebbles) {
        fprintf(stderr, "Error allocating memory\r\n");
        return 1;
    }
    size_t count    = 0;
    size_t capacity = sizeof(pebbles) / sizeof(pebbles[0]);

    while (count < capacity && fscanf(file, "%" PRIu64, &pebbles[count]) == 1) {
        count++;
    }

    printf("Found %zu Pebbles\r\n", count);

    for (size_t i = 0; i < count; ++i) {
        printf("%" PRIu64 " ", pebbles[i]);
    }

    free(pebbles);
    fclose(file);
    return 0;
}
