#include <fcntl.h>

#define BUFSIZ 4096

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]) {

    if(argc < 2){
        write(1, "Basic usage: cat [file_name]\n", sizeof("Basic usage: cat [file_name]\n"));
        return 1;
    }

    char buf[BUFSIZ];
    int fd;
    fd = open(argv[1], O_RDONLY, 0);

    if(fd == -1){
        char *t;
        write(2, "cat: ", sizeof("cat: "));
        /* t = argv[1] copies the pointer value of argv[1] to t
         * *t will check if current character is null
         * t++ moves char forward one byte in memory */
        for(t = argv[1]; *t != '\0'; t++ ){
            write(2, t, 1);
        }
        write(2, ": No such file or directory\n",  sizeof(": No such file or directory\n"));
        return 1;
    }

    int n;
    while( (n = read(fd, buf,BUFSIZ)) > 0){
        write(1, buf, n);
    }
    write(1, "\n", 1);

    return 0;
}
