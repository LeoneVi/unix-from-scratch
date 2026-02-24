#include <fcntl.h>
#include <sys/syscall.h>

#define BUFSIZ 4096

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]) {

    char buf[BUFSIZ];
    int fd;
    fd = open(argv[1], O_RDONLY, 0);

    if(fd == -1){
        write(2, "cat: ", sizeof("cat: "));
        write(2, argv[1], sizeof(argv[1]));
        write(2, ": No such file or directory\n",  sizeof(": No such file or directory\n"));
    }

    int n;
    while( (n = read(fd, buf,BUFSIZ)) > 0){
        write(1, buf, n);
    }

    return 0;
}
