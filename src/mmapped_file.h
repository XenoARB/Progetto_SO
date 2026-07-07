#pragma once
#include <stddef.h>

typedef struct {
    int fd;
    void *mem;
    size_t size;
}MmappedFile;

int mapped_file_open(MmappedFile *mf, const char *path);

int mapped_file_map(MmappedFile *mf);

void mapped_file_close(MmappedFile *mf);
