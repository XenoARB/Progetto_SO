#pragma once

#include <stdint.h>

#define BLOCK_SIZE 4096
#define FS_MAGIC 0x534F4653

typedef struct{
    uint32_t magic;
    uint32_t block_size;
    uint32_t num_blocks;
    uint32_t root_block;
    uint32_t head;
}Superblock;


