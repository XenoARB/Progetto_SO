#include "mmapped_file.h"
#include "fs_format.h"
#include "fs_debug.h"
#include "block_alloc.h"
#include "fs_types.h"

#include <stdio.h>

int main(int argc, char **argv) {
    const char *path = argv[1];
    size_t size = 40960; 
    fs_format(path, size);

    MmappedFile mf;
    mf_open(&mf, path);
    mf_map(&mf);

    uint32_t allocated[16];
    int count = 0;

    uint32_t idx;
    while ((idx = alloc_block(&mf)) != BLOCK_NONE) {
        allocated[count++] = idx;
    }
    printf("blocchi allocati: %d\n", count);

    uint32_t extra = alloc_block(&mf);
    printf("alloc_block a disco pieno ritorna: %u\n", extra);

    for (int i = 0; i < count; i++) {
        free_block(&mf, allocated[i]);
    }

    Superblock *sb = (Superblock *) mf.mem;
    print_superblock(&mf);
    print_chain(&mf, sb->head);

    mf_close(&mf);

    return 0;
}