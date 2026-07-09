#include "shell.h"
#include <stdio.h>
#include <string.h>


typedef void (*CommandFn)(int argc, char **args);

static void cmd_format(int argc, char **args) {printf("format placeholder");}
static void cmd_mkdir(int argc, char **args)  {printf("mkdir placeholder");}
static void cmd_cd(int argc, char **args)     {printf("cd placeholder");}
static void cmd_touch(int argc, char **args)  {printf("touch placeholder");}
static void cmd_cat(int argc, char **args)    {printf("cat placeholder");}
static void cmd_ls(int argc, char **args)     {printf("ls placeholder");}
static void cmd_append(int argc, char **args) {printf("append placeholder");}
static void cmd_rm(int argc, char **args)     {printf("rm placeholder");}
static void cmd_close(int argc, char **args)  {printf("close placeholder");}

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