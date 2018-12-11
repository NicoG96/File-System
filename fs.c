#include "fs.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>

void map() {
    if(!(fs = open("drive", O_RDWR))){
        perror("Error opening drive");
        exit(EXIT_FAILURE);
    }

    //begin with superblock
    sb = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, fs, 0);
    if(sb == MAP_FAILED){
        close(fs);
        perror("Error mmapping superblock");
        exit(EXIT_FAILURE);
    }

    //then inode bitmap
    //imap = mmap(NULL,sizeof(imap), PROT_READ | PROT_WRITE , MAP_SHARED , fs, 512);

    //then data bitmap
    //dmap = mmap(NULL,sizeof(bmap), PROT_READ | PROT_WRITE , MAP_SHARED , fs, sizeof(sb) + sizeof(imap));

    //then inode group
    //then directory folder
    //finally the data blocks


    //initialize SB
    superblock_init(sb);

    fsync(fs);
    close(fs);

}

void superblock_init(superblock *sb) {
    sb->block_size = BLOCK_SIZE;
    sb->num_blocks = MAX_BLOCKS;


    sb->numiblocks = 1;
    sb->numinodes = 2;
    sb-> i_size = 3;

    sb->dmap = 4;
    sb->imap = 5;
    sb->data = 6;

    sb->dmap_size = 7;
    sb->imap_size = 8;
    sb->data_size = 9;

    sb->root_dir = 10;
}

void getDatetime(char *dateTime) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *buf = asctime(tm);
    strncpy(dateTime, buf, 25); /* 25 bytes is len of asctime return */
}

void directory_init(directory *root) {}

/*
 * void disk_read( int blocknum, char *data )
{
	sanity_check(blocknum,data);

	fseek(diskfile,blocknum*DISK_BLOCK_SIZE,SEEK_SET);

	if(fread(data,DISK_BLOCK_SIZE,1,diskfile)==1) {
		nreads++;
	} else {
		printf("ERROR: couldn't access simulated disk: %s\n",strerror(errno));
		abort();
	}
}
 */