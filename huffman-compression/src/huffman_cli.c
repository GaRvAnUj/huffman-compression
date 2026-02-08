#include "huffman.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s encode|decode input_file output_file\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "encode") == 0) {
        encodeFile(argv[2], argv[3]);
    } else if (strcmp(argv[1], "decode") == 0) {
        decodeFile(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return 1;
    }
    return 0;
}
