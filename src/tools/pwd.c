#include <fcntl.h>
#include <sys/syscall.h>

#define BUFSIZ 4096

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]) {

    char buf[BUFSIZ];

    return 0;
}
