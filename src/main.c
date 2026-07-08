#include <stdio.h>
#include "mmapped_file.h"

int main(int argc, char **argv){
    MmappedFile mf;
    mf_open(&mf, argv[1]);

    fprintf(stderr, "argc: %d\n", argc);

    if(argc<3){
        size_t size=8192;
        mf_resize(&mf, size);

        unsigned char *c = (unsigned char *) mf.mem;
        fprintf(stderr, "scrivo \n");
        for(size_t i=0; i<size; i++) c[i]=(unsigned char) i;
        
    }
    else{
        mf_map(&mf);

        fprintf(stderr, "leggo \n");
        unsigned char *c=(unsigned char *) mf.mem;
        for(size_t i=0; i<mf.size; i++) printf("%d ", (int) c[i]);
        printf("\n");
    }

    mf_close(&mf);
    return 0;
}