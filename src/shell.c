#include "shell.h"
#include "mmapped_file.h"
#include "fs_format.h"
#include "fs_types.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint32_t cwd;


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








static void cmd_mkdir(int argc, char **args)  {printf("mkdir placeholder\n");}
static void cmd_cd(int argc, char **args)     {printf("cd placeholder\n");}
static void cmd_touch(int argc, char **args)  {printf("touch placeholder\n");}
static void cmd_cat(int argc, char **args)    {printf("cat placeholder\n");}
static void cmd_ls(int argc, char **args)     {printf("ls placeholder\n");}
static void cmd_append(int argc, char **args) {printf("append placeholder\n");}
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

