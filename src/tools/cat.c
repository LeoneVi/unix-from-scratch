#include <fcntl.h>

#define BUFSIZ 4096
#define BASIC_USAGE "Basic usage: cat [file_name]\n"
#define CAT "cat: "
#define NO_DIRECTORY ": No such file or directory\n"

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]) {

    if(argc < 2){
        write(1, BASIC_USAGE, sizeof(BASIC_USAGE));
        return 1;
    }

    char buf[BUFSIZ];
    int fd;
    fd = open(argv[1], O_RDONLY, 0);

    if(fd == -1){
        char *t;
        write(2, CAT, sizeof(CAT));
        /* t = argv[1] copies the pointer value of argv[1] to t
         * *t will check if current character is null
         * t++ moves char forward one byte in memory */
        for(t = argv[1]; *t != '\0'; t++ ){
            write(2, t, 1);
        }
        write(2, NO_DIRECTORY,  sizeof(NO_DIRECTORY));
        return 1;
    }

    int n;
    while( (n = read(fd, buf,BUFSIZ)) > 0){
        write(1, buf, n);
    }
    write(1, "\n", 1);

    return 0;
}
