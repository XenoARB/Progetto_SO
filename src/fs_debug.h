#pragma once

#include <stdint.h>

#include "mmapped_file.h"

void print_superblock(MmappedFile *mf);
void print_chain(MmappedFile *mf, uint32_t start);