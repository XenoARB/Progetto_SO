#include "block_alloc.h"
#include "fs_types.h"

uint32_t alloc_block(MmappedFile *mf){
    Superblock *sb= (Superblock *) mf->mem;

    uint32_t index=sb->head;
    if (index==BLOCK_NONE) return BLOCK_NONE;

    Blockheader *bh= (Blockheader *) ((char *) mf->mem + index * BLOCK_SIZE);
    sb->head=bh->next;

    return index;
}

void free_block(MmappedFile *mf, uint32_t index){
    Superblock *sb= (Superblock *) mf->mem;

    Blockheader *bh= (Blockheader *) ((char *) mf->mem + index * BLOCK_SIZE);
    bh->next=sb->head;
    sb->head=index;
}