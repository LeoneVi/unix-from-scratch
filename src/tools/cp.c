#include <fcntl.h>

#define BUFSIZ 4096
#define PERMS 0666
#define BASIC_USAGE "Basic usage: cp [source_file] [target_file]\n"
#define CP "cp :"
#define NO_DIRECTORY ": No such file or directory\n"

int read(int fd, char *buf, int n);
int write(int fd, char *buf, int n);

int main(int argc, char *argv[]){

    if(argc < 3){
        write(1, BASIC_USAGE, sizeof(BASIC_USAGE));
        return 1;
    }

    char buf[BUFSIZ];
    int source, target, n;

    source = open(argv[1], O_RDONLY, 0);

    if(source == -1){
        char *t;
        write(2, CP, sizeof(CP));
        for(t = argv[1]; *t != '\0'; t++ ){
            write(2, t, 1);
        }
        write(2, NO_DIRECTORY,  sizeof(NO_DIRECTORY));
        return 1;
    }

    target = creat(argv[2], PERMS);

    while( (n = read(source, buf,BUFSIZ)) > 0){
        write(target, buf, n);
    }

    return 0;
}