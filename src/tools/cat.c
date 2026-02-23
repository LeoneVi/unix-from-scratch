#include <fcntl.h>

#define BUFSIZ 4096

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]) {

    char buf[BUFSIZ];
    int fd;
    fd = open(*argv, O_RDONLY, 0);
    int n;

    while( (n = read(fd, buf,  BUFSIZ)) > 0){
        write(1, buf, n);
    }

    return 0;
}
