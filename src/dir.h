#pragma once

#include "mmapped_file.h"
#include "fs_types.h"

DirEntry *find(MmappedFile *mf, uint32_t first_block, const char *name);

uint32_t new_dirblock(MmappedFile *mf);
DirEntry *add_direntry(MmappedFile *mf, uint32_t first_block);
