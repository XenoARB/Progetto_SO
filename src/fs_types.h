#pragma once

#include <stdint.h>

#define BLOCK_SIZE 4096
#define FS_MAGIC 0x534F4653
#define BLOCK_NONE 0

typedef struct{
    uint32_t magic;
    uint32_t block_size;
    uint32_t num_blocks;
    uint32_t root_block;
    uint32_t head;
}Superblock;

typedef struct{
    uint32_t next;
}Blockheader;

#define ENTRY_FREE 0
#define ENTRY_FILE 1
#define ENTRY_DIR 2
#define NAME_MAX_LEN 28

typedef struct{
    char name[NAME_MAX_LEN];
    uint8_t type;
    uint32_t first_block;
    uint32_t size;
}DirEntry;


#define ENTRY_BLOCK ((BLOCK_SIZE - sizeof(Blockheader))) / sizeof(DirEntry)


