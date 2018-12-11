#include <sys/stat.h>
#include <ctype.h>
#include "fs.h"

int main(int argc, char *argv[]) {
    if(argc > 2) {
        perror("Too many arguments");
        exit(EXIT_FAILURE);
    }

    if(argc == 2) {
        //open existing fs
        if(!(fs = fopen(argv[1], "r+"))) {

            //if it doesn't exist, make one
            if(!(fs = fopen(argv[1], "w+"))) {


                //change to function



                perror("File System open");
                exit(EXIT_FAILURE);
            }

        }
    }

    int done = 0;
    char *input = malloc(sizeof(input));
    char filename[MAX_FILE_NAME];
    print_manual();

    while(!done) {
        //get user input
        fgets(input, 128, stdin);

        //remove newline
        if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n')) {
            input[strlen(input) - 1] = '\0';
        }

        /* FUNCTIONS */
        if (strncmp(input, "mkdir", 5) == 0) {
            //parse input to get filename
            if(get_file_name(input, filename)) {

                //if user typed mkdir without specifying filename, null is returned
                puts("Please specify a directory name.");
                continue;
            }

            //mkdir
        }

        else if(strncmp(input, "newfile", 7) == 0) {
            if(get_file_name(input, filename)) {
                puts("Please specify a file name.");
                continue;
            }

            //makefile
        }

        else if(strncmp(input, "open", 4) == 0) {
            if(get_file_name(input, filename)) {
                puts("Please specify the folder to open.");
                continue;
            }
            //open file
        }

        else if(strcmp(input, "close") == 0) {
            //close file
        }

        else if(strcmp(input, "read") == 0) {
            //read file
        }

        else if(strcmp(input, "write") == 0) {
            //write to file
        }

        else if(strncmp(input, "delete", 6) == 0) {
            if(get_file_name(input, filename)) {
                puts("Please specify the file to delete.");
                continue;
            }
            //delete file
        }

        else if(strcmp(input, "list") == 0) {
            //print dir
        }

        else if(strcmp(input, "help") == 0) {
            print_manual();
        }

        else if(strcmp(input, "exit") == 0) {
            done = 1;
        }

        else {
            puts("Command not recognized.");
        }

        printf("%s\n", filename);
    }

    free(input);
    return 0;
}

int get_file_name(char *input, char filename[]) {
    int i = 0;

    //parse input until there's a space, thus indicating a filename
    while((!(isspace(input[i]))) && input[i] != '\0') {
        i++;
    }

    //if there wasn't a space in the input, there is no name to attach to the file, return a failure
    if(!isspace(input[i])) return 1;
    input++;

    int dirptr = 0;

    //find name of file by scanning characters after 'mkdir' and adding them to a diff char array
    while(input[i] != '\0' && dirptr < MAX_FILE_NAME) {
        filename[dirptr++] = input[i++];
    }

    filename[dirptr] = '\0';

    return 0;
}

void print_manual() {
    puts("==============================================================");
    puts("Filesystem Operations:");
    printf("\t-%s\n", "mkdir [name]:\t\tCreates a new directory");
    printf("\t-%s\n", "newfile [name]:\tCreates a new file");
    printf("\t-%s\n", "open [directory]:\tOpens specified folder");
    printf("\t-%s\n", "delete [file]:\t\tDeletes the specified file");
    printf("\t-%s\n", "close:\t\t\tCloses currently open folder");
    printf("\t-%s\n", "read:\t\t\tReads the currently open file");
    printf("\t-%s\n", "write:\t\t\tWrites to currently open file");
    printf("\t-%s\n", "list:\t\t\tLists the items in the current directory");
    printf("\t-%s\n", "help:\t\t\tDisplay this prompt");
    printf("\t-%s\n", "exit:\t\t\tExits the filesystem");
    puts("==============================================================");
}