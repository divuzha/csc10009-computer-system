// xargs.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char *program, char **args) {
    if(fork() == 0) { // child exec
        exec(program, args);
        exit(0);
    }
    return;
}

int main(int argc,char * argv[]) {

    if (argc < 2){
        printf("usage: xargs  <command>  [command args]...\n");
        exit(1);
    }

    char buf[2048];
    char *argsbuf[128];
    char **args = argsbuf;

    for (int i = 1; i < argc; i++) {
        *args = argv[i];
        args++;
    }

    char *p = buf, *pbgin = buf;
    char **pargs = args;
    while (read(0, p, 1) != 0) {
        if (*p == ' ' || *p == '\n') {
            *p = '\0';
            *(pargs++) = pbgin;
            pbgin = p+1;

            if (*p == '\n') {
                *pargs = 0;
                run(argv[1],argsbuf);
                pargs = args;
            }
        }
        p++;
    }

    if (pargs != args){
        *p = '\0';
        *(pargs++) = pbgin;
        *pargs = 0;
        run(argv[1],argsbuf);
    }

    while (wait(0) != -1);
    exit(0);
}
