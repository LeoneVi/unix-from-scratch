#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 8

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

int isCommand(char *arg){
    if(strcmp(arg, "-n") == 0 || strcmp(arg, "-c") == 0){
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]){

    switch(argc){
        case 0:
        case 1:
            fprintf(stderr, "Basic usage: head [-n count | -c bytes] [file ...]\n");
            exit(1);
        case 2:
            char *arg1 = (char *)malloc(sizeof(argv[1]));
            strcpy(arg1, argv[1]);

            if(isCommand(arg1)){
                if(argc == 2){
                    arg1++;
                    fprintf(stderr, "head: option requires an argument -- %s\n"
                                    "usage: head [-n lines | -c bytes] [file ...]\n", arg1);
                    free(arg1);
                    exit(1);
                }

            } else {
                if(arg1[0] == '-' && strlen(arg1) > 1){ // if user enters command that does not exist
                    arg1++;
                    fprintf(stderr, "head: invalid option -- %s\n"
                                    "usage: head [-n lines | -c bytes] [file ...]\n", arg1);
                    free(arg1);
                    exit(1);
                }

                // When user only inputs a file, head prints the first 10 lines.
                printLines(argv[1], 10);
            }
    }
}