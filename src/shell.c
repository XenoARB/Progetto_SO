#include "shell.h"
#include "mmapped_file.h"
#include "fs_format.h"
#include "fs_types.h"
#include "dir.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint32_t cwd;
#define MAX_DEPTH 64
static int depth=0;
static uint32_t path_stack[MAX_DEPTH];


typedef void (*CommandFn)(int argc, char **args);



static MmappedFile mf;
static int fs_open = 0;

static void cmd_format(int argc, char **args){
    if(!(argc==3)){
        printf("usare il formato: format <file> <size>\n");
        return;
    }
    if(fs_open){
        printf("fs già aperto\n");
        return;
    }

    const char *path=args[1];
    size_t size=(size_t) strtoul(args[2], NULL, 10);

    int ret=fs_format(path, size);
    if(ret<0){
        printf("errore format\n");
        return;
    }

    ret=mf_open(&mf, path);
    if(ret<0){
        printf("errore di apertura\n");
        return;
    }
    ret=mf_map(&mf);
    if(ret<0){
        printf("errore di mappatura\n");
        return;
    }

    fs_open=1;
    Superblock *sb = (Superblock *) mf.mem;
    cwd=sb->root_block;
    printf("file system formattato e aperto\nfs_path: %s\n", path);
    return;
}

static void cmd_close(int argc,char **args){
    if(!fs_open){
        printf("fs non aperto\n");
        return;
    }
    else{
        mf_close(&mf);
        fs_open=0;
        printf("fs chiuso\n");
        return;
    }
}

static void cmd_mkdir(int argc, char **args){
    if(argc!=2){
        printf("usare il formato: mkdir <nome>\n");
        return;
    }
    if(!fs_open){
        printf("fs non aperto\n");
        return;
    }
    if(find(&mf, cwd, args[1])!=NULL){
        printf("esiste già una voce con questo nome\n");
        return;
    }

    uint32_t dirblock= new_dirblock(&mf);
    if(dirblock== BLOCK_NONE){
        printf("spazio esaurito\n");
        return;
    }

    DirEntry *slot = add_direntry(&mf, cwd);
    if(slot==NULL){
        free_block(&mf, dirblock);
        printf("spazio esaurito\n");
        return;
    }

    strncpy(slot->name, args[1], NAME_MAX_LEN-1);
    slot->name[NAME_MAX_LEN-1] ='\0';
    slot->type=ENTRY_DIR;
    slot->first_block=dirblock;
    slot->size=0;

    printf("%s creata\n", args[1]);
}


static void cmd_touch(int argc, char **args){
    if(argc!=2){
        printf("usare il formato: touch <nome>\n");
        return;
    }
    if(!fs_open){
        printf("fs non aperto\n");
        return;
    }
    if(find(&mf, cwd, args[1])!=NULL){
        printf("esiste gia una voce con queto nome\n");
        return;
    }

    DirEntry *slot=add_direntry(&mf, cwd);
    if(slot==NULL){
        printf("spazio esaurito\n");
        return;
    }

    strncpy(slot->name, args[1], NAME_MAX_LEN-1);
    slot->name[NAME_MAX_LEN-1] ='\0';
    slot->type=ENTRY_FILE;
    slot->first_block=BLOCK_NONE;
    slot->size=0;

    printf("%s creato\n", args[1]);
    return;
}

static void cmd_ls(int argc, char **args){
    if(argc!=1){
        printf("usa ls senza argomenti");
        return;
    }
    if(!fs_open){
        printf("fs non aperto\n");
        return;
    }

    uint32_t block=cwd;
    while(block!=BLOCK_NONE){
        char *base=(char*) mf.mem + block*BLOCK_SIZE;
        DirEntry *entry=(DirEntry *)(base+sizeof(Blockheader));

        for(uint32_t i=0; i<ENTRY_BLOCK; i++){
            if(entry[i].type==ENTRY_FREE) continue;
            const char *tipo=(entry[i].type==ENTRY_DIR) ? "dir" : "file";
            printf("%s \t\t<%s>\n", entry[i].name, tipo);
        }
    Blockheader *bh=(Blockheader *) base;
    block=bh->next;
    }
}


static void cmd_cd(int argc, char **args){
    if(argc!=2){
        printf("usare il formato: cd <directory>\n");
        return;
    }
    if(!fs_open){
        printf("fs non aperto\n");
        return;
    }

    uint32_t tmp_cwd;
    int tmp_depth;
    uint32_t tmp_stack[MAX_DEPTH];

    if(args[1][0]=='/'){
        Superblock *sb= (Superblock *) mf.mem;
        tmp_cwd=sb->root_block;
        tmp_depth=0;
    }
    else{
        tmp_cwd=cwd;
        tmp_depth=depth;
        memcpy(tmp_stack, path_stack, depth*sizeof(uint32_t));
    }

    char *token=strtok(args[1], "/");
    while(token!=NULL){
        if(strcmp(token, "..")==0){
            if(tmp_depth==0){
                printf("sei già nella radice\n");
                return;
            }
            tmp_cwd=tmp_stack[--tmp_depth];
        } 
        else {
            DirEntry *entry=find(&mf, tmp_cwd, token);
            if(entry==NULL){
                printf("%s non esiste\n", token);
                return;
            }
            if(entry->type!=ENTRY_DIR){
                printf("%s non è una directory\n", token);
                return;
            }
            if(tmp_depth>=MAX_DEPTH){
                printf("percorso troppo profondo\n");
                return;
            }
            tmp_stack[tmp_depth++]=tmp_cwd;
            tmp_cwd=entry->first_block;
        }
        token=strtok(NULL, "/");
    }
    cwd=tmp_cwd;
    depth=tmp_depth;
    memcpy(path_stack, tmp_stack, tmp_depth*sizeof(uint32_t));
}


static void cmd_append(int argc, char **args){
    if(argc<3){
        printf("usare il formato: append <file> <testo>\n");
        return;
    }
    if(!fs_open){
        printf("fs non aperto\n");
        return;
    }

    DirEntry *entry=find(&mf, cwd, args[1]);
    if(entry==NULL){
        printf("%s non esiste in questa directory\n", args[1]);
        return;
    }
    if(entry->type!=ENTRY_FILE){
        printf("%s non è un file\n", args[1]);
        return;
    }

    char text[256]="";
    for(int i=2; i<argc; i++){
        strcat(text, args[i]);
        if(i<argc-1) strcat(text, " ");
    }

    const uint32_t max= BLOCK_SIZE-sizeof(Blockheader);
    uint32_t last;
    uint32_t used;

    if(entry->first_block==BLOCK_NONE){
        last=alloc_block(&mf);
        if(last==BLOCK_NONE){
            printf("spazio esaurito\n");
            return;
        }
        Blockheader *bh=(Blockheader *)((char *) mf.mem + last*BLOCK_SIZE);
        bh->next=BLOCK_NONE;
        entry->first_block=last;
        used=0;
    } 
    else {
        last=entry->first_block;
        Blockheader *bh=(Blockheader *)((char *) mf.mem + last*BLOCK_SIZE);
        while(bh->next!=BLOCK_NONE){
            last=bh->next;
            bh=(Blockheader *)((char *) mf.mem + last*BLOCK_SIZE);
        }
        used=entry->size % max;
        if(entry->size>0 && used==0){
            uint32_t nb=alloc_block(&mf);
            if(nb==BLOCK_NONE){
                printf("spazio esaurito\n");
                return;
            }
            bh->next=nb;
            Blockheader *nbh=(Blockheader *)((char *) mf.mem + nb*BLOCK_SIZE);
            nbh->next=BLOCK_NONE;
            last=nb;
            used=0;
        }
    }

    size_t len=strlen(text);
    size_t written=0;
    while(written<len){
        char *base=(char *) mf.mem + last*BLOCK_SIZE;
        char *content=base+sizeof(Blockheader);
        size_t space=max-used;
        size_t chunk=(len-written<space) ? (len-written) : space;

        memcpy(content+used, text+written, chunk);
        written+=chunk;
        used+=chunk;

        if(written<len){
            uint32_t nb=alloc_block(&mf);
            if(nb==BLOCK_NONE){
                printf("spazio esaurito, scritti %zu byte su %zu\n", written, len);
                break;
            }
            Blockheader *bh=(Blockheader *) base;
            bh->next=nb;
            Blockheader *nbh=(Blockheader *)((char *) mf.mem + nb*BLOCK_SIZE);
            nbh->next=BLOCK_NONE;
            last=nb;
            used=0;
        }
    }

    entry->size += written;
    printf("%zu byte aggiunti a %s\n", written, args[1]);
    

}

static void cmd_cat(int argc, char **args)    {printf("cat placeholder\n");}
static void cmd_rm(int argc, char **args)     {printf("rm placeholder\n");}

typedef struct{
    const char *name;
    CommandFn fn;
}Command;

static Command command[]={
    {"format", cmd_format},
    {"mkdir", cmd_mkdir},
    {"cd", cmd_cd},
    {"touch", cmd_touch},
    {"cat", cmd_cat},
    {"ls", cmd_ls},
    {"append", cmd_append},
    {"rm", cmd_rm},
    {"close", cmd_close}
};

#define N_COMM 9

void shell_dispatch(int argc, char **args){
    for (size_t i=0; i<N_COMM; i++){
        if(strcmp(args[0], command[i].name)==0){
            command[i].fn(argc, args);
            return;
        }
    }
    printf("comando %s non esiste\n ",args[0]);
}

