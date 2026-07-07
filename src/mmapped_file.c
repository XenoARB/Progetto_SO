#include "mmapped_file.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

int mapped_file_open(MmappedFile *mf, const char *path) {
    mf->fd = open(path, O_RDWR | O_CREAT, 0660);
    if (mf->fd == -1) {
        perror("Error opening file");
        return -1;
    }

    struct stat st;
    if(fstat(mf->fd, &st) <0){
        perror("fstat");
        close(mf->fd);
        return -1;
    }

    mf->size = st.st_size;
    mf->mem =NULL;

    return 0;
}

int mapped_file_map(MmappedFile *mf){
    if (mf->size == 0){
        mf->mem = NULL;
        return 0;
    }

    mf->mem = mmap(
        NULL,
        mf->size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        mf->fd,
        0
    );
    if(mf->mem==MAP_FAILED){
        perror("mmap");
        mf->mem=NULL;
        return -1;
    }

    return 0;
}

void mapped_file_close(MmappedFile *mf){
    if(mf->mem){
        munmap(mf->mem, mf->size);
    }
    close(mf->fd);
}