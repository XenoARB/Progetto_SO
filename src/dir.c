#include "dir.h"
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