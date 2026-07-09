#include "fs_format.h"
#include "fs_types.h"
#include "mmapped_file.h"
#include "block_alloc.h"

int fs_format(const char *path, size_t size){
    
    MmappedFile mf;
    
    int ret=mf_open(&mf, path);
    if(ret<0) return -1;

    uint32_t num_blocks = size / BLOCK_SIZE;
    size_t a_size=(size_t) num_blocks * BLOCK_SIZE;

    ret=mf_resize(&mf, a_size);
    if(ret<0){
        mf_close(&mf);
        return -1;
    }

    Superblock *sb= (Superblock *) mf.mem;
    sb->magic = FS_MAGIC;
    sb->block_size = BLOCK_SIZE;
    sb->num_blocks = num_blocks;
    sb->head = (num_blocks>1) ? 1 : BLOCK_NONE;

    for(uint32_t i=1; i<num_blocks; i++){
        Blockheader *bh= (Blockheader *) ((char *) mf.mem + i * BLOCK_SIZE);
        bh->next = (i + 1<num_blocks) ? (i+1) : BLOCK_NONE;
    }

    uint32_t root=alloc_block(&mf);
    Blockheader *r_bh= (Blockheader *) ((char *) mf.mem + root * BLOCK_SIZE);
    r_bh->next=BLOCK_NONE;

    DirEntry *entry= (DirEntry *) ((char *) r_bh + sizeof(Blockheader));
    for(uint32_t i=0; i<ENTRY_BLOCK; i++) entry[i].type=ENTRY_FREE;

    sb->root_block=root;

    mf_close(&mf);

    return 0;
}