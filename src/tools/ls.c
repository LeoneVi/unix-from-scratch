#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define EMPTY (void *)0

int write(int fd, char *buf, int n);

int main(int argc, char *argv[]){
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir("."); // open current directory

    while((dp = readdir(dirp)) != EMPTY){
        if(strcmp(dp->d_name, ".") == 0 ||
           strcmp(dp->d_name, "..") == 0 ||
           dp->d_name[0] == '.'
        ){
            continue;
        }
        printf("%s        ", dp->d_name);
    }

    printf("\n");
    closedir(dirp);
}