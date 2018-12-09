#include "fs.h"

int main(int argc, char **argv) {
    if(argc > 2) {
        perror("Too many arguments");
        exit(EXIT_FAILURE);
    }

    //open the file system
    if(argc == 2) {
        FILE *fs;
        if(!(fs = fopen(argv[1], "w+"))) {
            perror("File System open");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}