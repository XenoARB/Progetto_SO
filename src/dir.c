#include "dir.h"
#include "block_alloc.h"

#include <string.h>

DirEntry *find(MmappedFile *mf, uint32_t first_block, const char *name){
    uint32_t block= first_block;

    while(block!=BLOCK_NONE){
        
        char *base= (char *) mf->mem + block * BLOCK_SIZE;
        DirEntry *entry = (DirEntry *) (base + sizeof(Blockheader));


        for (uint32_t i=0; i<ENTRY_BLOCK; i++){
            if(entry[i].type != ENTRY_FREE && strcmp(entry[i].name, name)==0) return &entry[i];
        }

        Blockheader *bh= (Blockheader *) base;
        block = bh->next;
    }
    return NULL;
}

uint32_t new_dirblock(MmappedFile *mf){
    uint32_t block=alloc_block(mf);
    if(block==BLOCK_NONE){
        return BLOCK_NONE;
    }

    Blockheader *bh= (Blockheader *) ((char *) mf->mem + block*BLOCK_SIZE);
    bh->next=BLOCK_NONE;

    DirEntry *entry= (DirEntry *) ((char *) bh + sizeof(Blockheader));
    for(uint32_t i=0; i<ENTRY_BLOCK; i++){
        entry[i].type=ENTRY_FREE;
    }
    return block;
}

DirEntry *add_direntry(MmappedFile *mf, uint32_t first_block){
    uint32_t block=first_block;
    uint32_t last_block=first_block;

    while(block!=BLOCK_NONE){
        char *base=(char *) mf->mem +block * BLOCK_SIZE;
        DirEntry *entry= (DirEntry *)(base+ sizeof(Blockheader));

        for(uint32_t i=0; i<ENTRY_BLOCK; i++){
            if(entry[i].type==ENTRY_FREE) return &entry[i];
        }
        last_block=block;
        Blockheader *bh=(Blockheader *) base;
        block= bh->next;
    }

    uint32_t new_block= new_dirblock(mf);
    if(new_block==BLOCK_NONE) return NULL;

    Blockheader *last_bh = (Blockheader *) ((char *) mf->mem +last_block * BLOCK_SIZE);
    last_bh->next = new_block;

    return (DirEntry *) ((char *) mf->mem + new_block * BLOCK_SIZE + sizeof(Blockheader));

}