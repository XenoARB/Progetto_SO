#pragma once

#include "mmapped_file.h"
#include "fs_types.h"

DirEntry *find(MmappedFile *mf, uint32_t first_block, const char *name);