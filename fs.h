/* LIBRARIES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/* MACROS */
#define BLOCK_SIZE 512
#define INODES_PER_BLOCK 6
#define POINTERS_PER_INODE 12
#define POINTERS_PER_BLOCK 128


/* STRUCTS */
typedef struct sb {
    unsigned block_size; //size of blocks
    unsigned fs_size;    //# of blocks (total)
    unsigned numfree;    //# of blocks free
    unsigned numiblocks; //# of inode blocks
    unsigned numinodes;  //# of inodes
    unsigned dmap;       //starting block of dmap
    unsigned imap;       //starting block of imap
    unsigned data;       //starting block of data table
    unsigned dmap_size;  //# of blocks in data bitmap
    unsigned imap_size;  //# of blocks in inode bitmap
    unsigned root_dir;   //starting block of root directory
}sb_t;

typedef struct inode {
    char *name;              //high-level name
    int isvalid;             //is this file in use?
    int isdirectory;         //is this file a directory?
    unsigned inum;           //low-level name
    unsigned size;           //# of blocks
    unsigned indirect;       //pointer to indirect inodes
    time_t creationdate;     //creation timestamp
    time_t lastmodified;     //last modification timestamp
    char *type;              //type of file
    unsigned direct[POINTERS_PER_INODE]; //pointers to data
}inode_t;

typedef struct dmap {

}dmap_t;

typedef struct imap {

}imap_t;

