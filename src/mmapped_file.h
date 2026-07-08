#pragma once
#include <stddef.h>

typedef struct {
    int fd;
    void *mem;
    size_t size;
}MmappedFile;

int mf_open(MmappedFile *mf, const char *path);

int mf_map(MmappedFile *mf);

void mf_close(MmappedFile *mf);

int mf_resize(MmappedFile *mf, size_t new_size);