#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

char* read_file_contents(const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "Could not open file\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = (char*)malloc(size + 1);
    size_t read_size = fread(buffer, 1, size, fp);
    fclose(fp);
    buffer[read_size] = '\0';
    return buffer;
}
