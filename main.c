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

    //boot up shell interface
    shell();

    return 0;
}

/*

void disk_read( int blocknum, char *data )
{

	fseek(diskfile,blocknum*DISK_BLOCK_SIZE,SEEK_SET);

	if(fread(data,DISK_BLOCK_SIZE,1,diskfile)==1) {
		nreads++;
	} else {
		printf("ERROR: couldn't access simulated disk: %s\n",strerror(errno));
		abort();
	}
}

void disk_write( int blocknum, const char *data )
{

	fseek(diskfile,blocknum*DISK_BLOCK_SIZE,SEEK_SET);

	if(fwrite(data,DISK_BLOCK_SIZE,1,diskfile)==1) {
		nwrites++;
	} else {
		printf("ERROR: couldn't access simulated disk: %s\n",strerror(errno));
		abort();
	}
}

int fs_format() {
    return 0;
}

int fs_mount() {
    return 0;
}

int fs_create() {
    return 0;
}

int fs_delete(int inumber) {
    return 0;
}

int fs_getsize(int inumber) {
    return -1;
}

int fs_read(int inumber, char *data, int length, int offset) {
    return 0;
}

int fs_write(int inumber, const char *data, int length, int offset) {
    return 0;
}
*/