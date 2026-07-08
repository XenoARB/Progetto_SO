#pragma once

#include <stdint.h>
#include "mmapped_file.h"

uint32_t alloc_block(MmappedFile *mf);

void free_block(MmappedFile *mf, uint32_t index);

