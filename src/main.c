#include "mmapped_file.h"
#include "fs_format.h"
#include "fs_debug.h"

int main(int argc, char **argv) {
    const char *path = argv[1];
    size_t size = 65536;  // 16 blocchi da 4096 byte

    fs_format(path, size);

    MmappedFile mf;
    mf_open(&mf, path);
    mf_map(&mf);

    print_superblock(&mf);
    print_chain(&mf, 1);

    mf_close(&mf);

    return 0;
}