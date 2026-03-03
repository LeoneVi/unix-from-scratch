#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFFERSIZE 8
#define NO_FILE "head: %s: No such file or directory\n"
#define BASIC_USAGE "Basic usage: head [-n count | -c bytes] [file ...]\n"
#define ILLEGAL_COUNT "head: illegal %s count -- %d\n"
#define CANT_COMBINE "head: can't combine line and byte counts\n"
#define REQUIRES_ARGUMENT "head: option requires an argument -- %s\nusage: head [-n lines | -c bytes] [file ...]\n"
#define INVALID_OPTION "head: invalid option -- %s\nusage: head [-n lines | -c bytes] [file ...]\n"

int handleFile(char *file){
    int fd;
    if((fd = open(file, O_RDONLY, 0)) == -1){
        fprintf(stderr, NO_FILE, file);
        exit(1);
    }
    return fd;
}

void printLines(int fd, int count){
    char buf[BUFFERSIZE];
    int newline = 0; // amount of \n we come across
    int n;
    int loop_complete = 0;
    while( (n = read(fd, buf, BUFFERSIZE)) > 0 && !loop_complete){
        for(int i = 0; i < n; i++){
            if(buf[i] == '\n') newline++;
            if(newline == count) {
                write(1, buf, i + 1);
                loop_complete = 1;
                break;
            }
        }
        if(newline != count) write(1, buf, n);
    }
}

void printBytes(int fd, int count){
    char buf[BUFFERSIZE];
    int byte_cnt = 0;
    int n;
    int loop_complete = 0;
    while( (n = read(fd, buf, BUFFERSIZE)) > 0 && !loop_complete){
        for(int i = 0; i < n; i++){
            byte_cnt++;
            if(byte_cnt == count) {
                write(1, buf, i + 1);
                loop_complete = 1;
                break;
            }
        }
        if(byte_cnt != count) write(1, buf, n);
    }
}

int isCommand(char *arg){
    if(strcmp(arg, "-n") == 0 || strcmp(arg, "-c") == 0){
        return 1;
    }
    return 0;
}

// if user enters command that does not exist
int commandDNE(char *arg){
    char *t = arg;
    if(t[0] == '-' && strlen(t) > 1){
        t++;
        fprintf(stderr, INVALID_OPTION, t);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr, BASIC_USAGE);
        exit(1);
    }

    int default_lines = 10;
    if(argc == 2) { // if user only inputted a file
        if(isCommand(argv[1])){
            char *t = argv[1];
            t++;
            fprintf(stderr, REQUIRES_ARGUMENT, t);
            exit(1);
        } else if(commandDNE(argv[1])){
            exit(1);
        }
        int fd = handleFile(argv[1]);
        printLines(fd, default_lines);
        close(fd);
        exit(0);
    }

    int intValue;
    int isN = 0;
    int isC = 0;
    for(int i = 1; i < argc; i++) {
        char *curr = argv[i];
        if(isCommand(curr)){ // PARSE COMMANDS

            if(strcmp(curr, "-n") == 0) isN = 1;
            else if(strcmp(curr, "-c") == 0) isC = 1;
            if(isN && isC){
                fprintf(stderr, CANT_COMBINE);
                exit(1);
            }

            if(i + 1 < argc){
                char *next = argv[i + 1];
                i += 1;
                if(commandDNE(next)){
                    exit(1);
                } else if (isdigit(*next)) {
                    intValue = atoi(next);
                    if(intValue == 0){
                        fprintf(stderr, ILLEGAL_COUNT, isN ? "line" : "byte", intValue);
                        exit(1);
                    }
                } else {
                    fprintf(stderr, BASIC_USAGE);
                    exit(1);
                }

            }
        } else if (commandDNE(curr)){
            exit(1);
        } else {
            // PARSING FILE INFO
            int fileCnt = argc - i;
            for(int j = 0; j < fileCnt; j++){
                int file = handleFile(argv[i + j]);
                if(fileCnt != 1) printf("==> %s <==\n", argv[i + j]);
                if(isN) printLines(file, intValue);
                else if(isC) printBytes(file, intValue);
                else printLines(file, default_lines);
                close(file);
                if(fileCnt != 1 && fileCnt - j != 1) printf("\n");
            }
            i += fileCnt;
        } // end of else
    } // end of for loop
    exit(0);
}
