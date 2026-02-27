#include <fcntl.h>

#define BUFSIZ 4096
#define PERMS 0666

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]){

    if(argc < 3){
        write(1, "Basic usage: cp [source_file] [target_file]\n", sizeof("Basic usage: cp [source_file] [target_file]\n"));
        return 1;
    }

    char buf[BUFSIZ];
    int source, target, n;

    source = open(argv[1], O_RDONLY, 0);

    if(source == -1){
        char *t;
        write(2, "cp: ", sizeof("cp: "));
        for(t = argv[1]; *t != '\0'; t++ ){
            write(2, t, 1);
        }
        write(2, ": No such file or directory\n",  sizeof(": No such file or directory\n"));
        return 1;
    }

    target = creat(argv[2], PERMS);

    while( (n = read(source, buf,BUFSIZ)) > 0){
        write(target, buf, n);
    }

    return 0;
}