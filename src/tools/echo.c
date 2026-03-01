int write(int fd, char *buf, int n);

int main(int argc, char *argv[]) {

    if(argc < 2){
        write(1, "Basic usage: echo [string]\n", sizeof("Basic usage: echo [string]\n"));
        return 1;
    }

    for(int i = 1; i < argc; i++){
        char* t;
        for(t = argv[i]; *t != '\0'; t++){
            write(1, t, 1);
        }
        write(1, " ", sizeof(" "));
    }
    write(1, "\n", 1);

    return 0;
}
