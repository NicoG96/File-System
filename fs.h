/* LIBRARIES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


/* MACROS */
#define TOTAL_BLOCKS 3906   /* blocks, floor of 2MB file / 512 */
#define MAX_FILE_SIZE 64    /* blocks */
#define BLOCK_SIZE 4096      /* bytes */
#define MAX_FILE_NAME 12
#define NUM_IBLOCKS 5
#define INODES_PER_BLOCK 4
#define DBLOCKS_PER_INODE 12
#define NUM_INODES 20


/* STRUCTS */
typedef struct superblock {
    unsigned block_size; //4- size of blocks
    unsigned num_blocks; //4- # of blocks (total)

    unsigned max_fsize;  //4- in bytes

    unsigned numiblocks; //4- # of inode blocks
    unsigned numinodes;  //4- # of inodes
    unsigned nodes_per_block; //4

    unsigned dmap;       //4- starting block of dmap
    unsigned imap;       //4- starting block of imap
    unsigned inodes;     //4- starting block for inodes
    unsigned data;       //4- starting block of data table

    unsigned dmap_size;  //4- # of blocks in data bitmap
    unsigned imap_size;  //4- # of blocks in inode bitmap
    unsigned data_size;  //4- # of blocks in inode bitmap
    unsigned inode_size; //4- size of inode block

    unsigned root_dir;   //4- starting block of root directory
}superblock;

typedef struct imap {
    _Bool occupied[BLOCK_SIZE];
}imap;

typedef struct dmap {
    _Bool occupied[BLOCK_SIZE];
}dmap;

typedef struct data {
    char d[27152];
}data;

typedef struct inode {
    _Bool isvalid;          //1- is inode in use?
    _Bool isdir;            //1- is the file a directory?
    int parent_dir;    //4- inode of parent directory

    int inum;          //4- low-level name
    char name[12];          //12- filename
    char extension[4];      //4- file extension

    int file_size;     //4- in bytes

    char creationdate[25];  //25- creation timestamp
    char lastmodified[25];  //25- last modification timestamp

    int data_ptr[DBLOCKS_PER_INODE]; //48- 12 pointers to file data
}inode;


/* GLOBALS */
int fs;
superblock *sb;
imap *im;
dmap *dm;
inode *in[NUM_INODES];
data *dat;
inode *curr_dir;


/* FUNCTIONS */
void print_manual();

void shell();
int get_file_name(char *input, char filename[]);

void map();
void superblock_init(superblock *sb);
void imap_init(imap *im);
void dmap_init(dmap *dm);
void inode_init(inode *in[]);
void inode_create(char *filename, int dataAddr, int free_node, _Bool isDir);
int find_free_inode(imap *im);
int find_free_datblock(dmap *dm);
void getDatetime(char *dateTime);

void mkdir(char *filename);
void newfile(char *filename);
int openf(char *filename);
int delete(char *filename);
int closef();
int readf(char *filename);
int writef(char *filename);
void list();
int update_dir(int inode);