#include "fs.h"

int main(int argc, char *argv[]) {
    if(argc > 2) {
        perror("Too many arguments");
        exit(EXIT_FAILURE);
    }

    //mount and map the disk
    map();

    //boot up shell interface
    shell();

    return 0;
}