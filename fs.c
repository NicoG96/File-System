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
    sb = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fs, 0);
    if(sb == MAP_FAILED){
        close(fs);
        perror("Error mapping superblock");
        exit(EXIT_FAILURE);
    }

    //then inode bitmap
    im = mmap(NULL, sizeof(imap), PROT_READ | PROT_WRITE, MAP_SHARED, fs, BLOCK_SIZE);
    if(im == MAP_FAILED){
        close(fs);
        perror("Error mapping inode bitmap");
        exit(EXIT_FAILURE);
    }

    //then data bitmap
    dm = mmap(NULL, sizeof(dmap), PROT_READ | PROT_WRITE , MAP_SHARED , fs, BLOCK_SIZE*2);
    if(dm == MAP_FAILED){
        close(fs);
        perror("Error mapping data bitmap");
        exit(EXIT_FAILURE);
    }

    //then inode group, 5 blocks, 4 inodes per block = 20 inodes
    for(int i = 0; i < 20; i++) {
        in[i] = mmap(NULL, sizeof(inode), PROT_READ | PROT_WRITE , MAP_SHARED , fs, (BLOCK_SIZE * 3) + (BLOCK_SIZE * i));
        if(in[i] == MAP_FAILED){
            close(fs);
            perror("Error mapping inode blocks");
            exit(EXIT_FAILURE);
        }
    }

    //finally the data blocks
    /* total bytes used by FS = 4096 -> TOTAL_BLOCKS*8 - 4096 = 27,152 */
    dat = mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE , MAP_SHARED , fs, BLOCK_SIZE * 24);
    if(dat == MAP_FAILED){
        close(fs);
        perror("Error mapping data blocks");
        exit(EXIT_FAILURE);
    }

    //initialize blocks
    superblock_init(sb);
    imap_init(im);
    dmap_init(dm);
    inode_init(in);

    //set up root folder
    newfile("root");

    //set current directory to root
    curr_dir = in[0];
}

void superblock_init(superblock *sb) {
    //blocks
    sb->block_size = BLOCK_SIZE;
    sb->num_blocks = TOTAL_BLOCKS;

    //files
    sb->max_fsize = MAX_FILE_SIZE * 8;

    //inodes
    sb->nodes_per_block = INODES_PER_BLOCK;
    sb->numiblocks = NUM_IBLOCKS;
    sb->numinodes = INODES_PER_BLOCK * NUM_IBLOCKS;

    //location in memory
    sb->imap = BLOCK_SIZE;
    sb->dmap = BLOCK_SIZE * 2;
    sb->inodes = BLOCK_SIZE * 3;
    sb->data = BLOCK_SIZE * 24;

    //sizes
    sb->imap_size = sizeof(imap);
    sb->dmap_size = sizeof(dmap);
    sb->inode_size = sizeof(inode);
    sb->data_size = sizeof(data);

    //root folder is first block
    sb->root_dir = sb->inodes;
}

void imap_init(imap *im) {
    for(int i = 0; i < BLOCK_SIZE; i++) {
        im->occupied[i] = 0;
    }
}

void dmap_init(dmap *dm) {
    for(int i = 0; i < BLOCK_SIZE; i++) {
        dm->occupied[i] = 0;
    }
}

void inode_init(inode *in[]) {
    for(int i = 0; i < NUM_INODES; i++) {
        in[i]->isvalid = 0;
        in[i]->isdir = 0;
        in[i]->parent_dir = -1;
        in[i]->inum = -1;
        in[i]->file_size = -1;
        for(int j = 0; j < DBLOCKS_PER_INODE; j++) {
            in[i]->data_ptr[j] = -1;
        }
    }
}

void inode_create(char *filename, int dataAddr, int free_node, _Bool isDir) {
    in[free_node]->isvalid = 1;
    in[free_node]->isdir = 1;

    strcmp(filename, "root") == 0 ?
    (in[free_node]->parent_dir = 0) :
    (in[free_node]->parent_dir = curr_dir->inum);

    in[free_node]->inum = (unsigned) free_node;
    in[free_node]->file_size = 0;
    strcpy(in[free_node]->name, filename);

    isDir ?
    strcpy(in[free_node]->extension, "dir") :
    strcpy(in[free_node]->extension, "txt");

    getDatetime(in[free_node]->creationdate);
    getDatetime(in[free_node]->lastmodified);
    in[free_node]->data_ptr[0] = (unsigned) dataAddr;
}

int find_free_datblock(dmap *dm) {
    //scan the iblock bitmap, find first available block
    for(int i = 0; i < BLOCK_SIZE; i++) {
        if(!dm->occupied[i]) {
            dm->occupied[i] = 1;
            return i;
        }
    }
    return -1;
}

int find_free_inode(imap *im) {
    for(int i = 0; i < BLOCK_SIZE; i++) {
        if(!im->occupied[i]) {
            im->occupied[i] = 1;
            return i;
        }
    }
    return -1;
}

void getDatetime(char *dateTime) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *buf = asctime(tm);
    strncpy(dateTime, buf, 25); /* 25 bytes is len of asctime return */
}