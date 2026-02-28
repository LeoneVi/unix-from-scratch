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
        for(int i = 0; i < n - 1; i++){
            printf("current buf: %c\n", buf[i]);
            if(buf[i] == '\n') newline++;
            printf("LINE COUNT: %d\n", newline);
            if(newline == count){ // if line count is reach, print everything we just read
                write(1, buf, i);
                loopComplete = 1;
            }
        }
        write(1, buf, n);
    }
}

int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr, "Basic usage: head [-n count | -c bytes] [file ...]\n");
        exit(1);
    }

    char *arg1 = (char *)malloc(sizeof(argv[1]));
    strcpy(arg1, argv[1]);
    if(strcmp(arg1, "-n") == 0 ||
       strcmp(arg1, "-c") == 0) // if user used command
    {

    } else {
        if(arg1[0] == '-' && strlen(arg1) > 1){ // if user enters command that does not exist
            arg1++;
            fprintf(stderr, "head: invalid option -- %s\nusage: head [-n lines | -c bytes] [file ...]\n", arg1);
            free(arg1);
            exit(1);
        }

        // When user only inputs a file, head prints the first 10 lines.
        printLines(argv[1], 10);
    }


}