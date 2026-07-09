#include <stdio.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_ARGS 16


int main() {
    char line[MAX_LINE];

    while(1){
        printf("> ");
        char *input=fgets(line, sizeof(line), stdin);
        
        if(input==NULL) break;

        line[strcspn(line, "\n")] = '\0';

        char *args[MAX_ARGS];
        int argc=0;

        char *token = strtok(line, " ");
        while(token != NULL && argc<MAX_ARGS){
            args[argc++] = token;
            token = strtok(NULL, " ");
        }

        if(argc==0) continue;

        printf("comando: %s (argc=%d)\n", args[0], argc);
    }

    return 0;
}