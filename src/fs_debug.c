#include "fs_debug.h"
#include "fs_types.h"

#include <stdio.h>

void print_superblock(MmappedFile *mf) {
    Superblock *sb = (Superblock *) mf->mem;

    printf("magic: 0x%x\n", sb->magic);
    printf("block_size: %u\n", sb->block_size);
    printf("num_blocks: %u\n", sb->num_blocks);
    printf("root_block: %u\n", sb->root_block);
    printf("head: %u\n", sb->head);
}

void print_chain(MmappedFile *mf, uint32_t start) {
    uint32_t i = start;

    printf("chain:");
    while (i != BLOCK_NONE) {
        printf(" %u", i);
        Blockheader *bh = (Blockheader *) ((char *) mf->mem + i * BLOCK_SIZE);
        i = bh->next;
    }
    printf("\n");
}