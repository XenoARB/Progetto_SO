#include "mmapped_file.h"
#include "fs_format.h"
#include"fs_debug.h"


int main(int argc, char **argv) {
    const char *path= argv[1];
    fs_format(path, 40960);
    
    MmappedFile mf;
    mf_open(&mf, path);
    mf_map(&mf);

    print_superblock(&mf);

    mf_close(&mf);
    return 0;
}