#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define BUFFERSIZE 8
#define BASIC_USAGE "Basic usage: head [-n count | -c bytes] [file ...]\n"

int handleFile(char *file){
    int fd;
    if((fd = open(file, O_RDONLY, 0)) == -1){
        fprintf(stderr, "head: %s: No such file or directory\n", file);
        exit(1);
    }
    return fd;
}

void printLines(char *file, int count){
    int fd = handleFile(file);
    char buf[BUFFERSIZE];
    int newline = 0; // amount of \n we come across
    int n;
    int loopComplete = 0;
    while( (n = read(fd, buf, BUFFERSIZE)) > 0 && loopComplete == 0){
        for(int i = 0; i < n; i++){
            if(buf[i] == '\n') newline++;
            if(newline == count){ // if line count is reach, print everything we just read
                write(1, buf, i);
                loopComplete = 1;
                break;
            }
        }
        if(newline != count) write(1, buf, n); // print every 8 bytes
    }
    write(1, "\n", sizeof("\n"));
}

void printBytes(char *file, int count){
    int fd = handleFile(file);
    char buf[BUFFERSIZE];
    int byte_cnt = 0;
    int n;
    int loopComplete = 0;
    while( (n = read(fd, buf, BUFFERSIZE)) > 0 && loopComplete == 0){
        for(int i = 0; i < n; i++){
            byte_cnt++;
            if(byte_cnt == count){ // if line count is reach, print everything we just read
                write(1, buf, i);
                loopComplete = 1;
                break;
            }
        }
        if(byte_cnt != count) write(1, buf, n); // print every 8 bytes
    }
    write(1, "\n", sizeof("\n"));
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
        fprintf(stderr, "head: invalid option -- %s\n"
                        "usage: head [-n lines | -c bytes] [file ...]\n", t);
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
    if(2 == argc) { // if user only inputted a file
        if(isCommand(argv[1])){
            char *t = argv[1];
            t++;
            printf("head: option requires an argument -- %s\nusage: head [-n lines | -c bytes] [file ...]\n", t);
            exit(1);
        } else if(commandDNE(argv[1])){
            exit(1);
        }
        printLines( argv[1], default_lines);
    }

    int intValue;
    int isN = 0;
    int isC = 0;
    for(int i = 1; i < argc; i++) {
        char *curr = argv[i];
        if(isCommand(curr)){ // PARSE COMMANDS
            if(i + 1 < argc){
                char *next = argv[i + 1];
                i += 1;
                if(commandDNE(next)){
                    exit(1);
                } else if (isdigit(*next)) {
                    intValue = *next - '0';
                } else {
                    fprintf(stderr, BASIC_USAGE);
                    exit(1);
                }

                if(strcmp(curr, "-n") == 0) isN = 1;
                else if(strcmp(curr, "-c") == 0) isC = 1;
                if(isN && isC){
                    fprintf(stderr, "head: can't combine line and byte counts\n");
                    exit(1);
                }
            }
        } else if (commandDNE(curr)){
            exit(1);
        } else {
            // Handle files
            int fileCnt = argc - i;
            char *file = argv[i];
            if(fileCnt == 0){
                fprintf(stderr, BASIC_USAGE);
                exit(1);
            } else if(fileCnt == 1){
                if(isN) printLines(file, intValue);
                else if(isC) printBytes(file, intValue);
                else printLines(file, default_lines);
            } else {
                for(int j = 0; j < fileCnt; j++){
                    printf("==> %s <==\n", file);
                    if(isN) printLines(file, intValue);
                    else if(isC) printBytes(file, intValue);
                    else printLines(file, default_lines);
                    i += j;
                }
            }
        }
    } // end of for loop

    exit(0);
}