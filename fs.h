/* LIBRARIES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


/* MACROS */
#define MAX_BLOCKS 3906     /* blocks, floor of 2MB file / 512 */
#define MAX_FILE_NAME 12    /* 8 for file, 4 for extension */
#define MAX_FILE_SIZE 64    /* blocks */
#define BLOCK_SIZE 512      /* bytes */
#define INODES_PER_BLOCK 6
#define POINTERS_PER_INODE 12
#define POINTERS_PER_BLOCK 128


/* GLOBALS */
FILE *fs;


/* STRUCTS */
typedef struct superblock {
    unsigned block_size; //size of blocks
    unsigned num_blocks; //# of blocks (total)
    unsigned num_free;   //# of blocks free

    unsigned numiblocks; //# of inode blocks
    unsigned numinodes;  //# of inodes
    unsigned i_size;     //size of inode

    unsigned dmap;       //starting block of dmap
    unsigned imap;       //starting block of imap
    unsigned data;       //starting block of data table

    unsigned dmap_size;  //# of blocks in data bitmap
    unsigned imap_size;  //# of blocks in inode bitmap

    unsigned root_dir;   //starting block of root directory
}superblock;


typedef struct inode {
    char *name;              //high-level name
    unsigned inum;           //low-level name
    int isvalid;             //is this file in use?
    int isdirectory;         //is this file a directory?
    int parent;              //parent directory
    unsigned size;           //size of file (bytes)
    unsigned indirect;       //pointer to indirect inodes
    time_t creationdate;     //creation timestamp
    time_t lastmodified;     //last modification timestamp
    char *type;              //type of file
    unsigned direct[POINTERS_PER_INODE]; //pointers to data
}inode;


/* FUNCTIONS */
void shell();
int get_file_name(char *input, char filename[]);
void print_manual();
void map();