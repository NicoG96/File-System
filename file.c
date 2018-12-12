#include "fs.h"

void mkdir(char *filename) {
    //find free inode
    int free_node = find_free_inode(im);

    /* each index in bitmap corresponds to an inode
     * get base address of inodes, add it to ith inode */
    int dblock = find_free_datblock(dm);
    int dataAddr = (dblock * BLOCK_SIZE + sb->data);

    //initialize inode
    int filenode = inode_init(filename, dataAddr, free_node, 1);

    //update directory list with new file
    if(update_dir(filenode)) {
        perror("Directory full");
    } else {
        printf("Directory '%s' successfully created\n", filename);
    }
}

void newfile(char *filename) {
    int free_node = find_free_inode(im);

    /* each index in bitmap corresponds to an inode
     * get base address of inodes, add it to ith inode */
    int dblock = find_free_datblock(dm);
    int dataAddr = (dblock * BLOCK_SIZE + sb->data);

    //initialize inode
    int filenode = inode_init(filename, dataAddr, free_node, 0);

    //update directory list with new file
    if(update_dir(filenode)) {
        perror("Directory full");
    } else {
        printf("File '%s' successfully created\n", filename);
    }
}

int openf(char *filename) {
    for(int i = 0; i < NUM_INODES; i++) {
        if(strcmp(filename, in[i]->name) == 0) {
            curr_dir = in[i];
            printf("Opened '%s'", filename);
            return 0;
        }
    }
    perror("File not found");
    return 1;
}

int delete(char *filename) {
    for(int i = 0; i < NUM_INODES; i++) {
        if(strcmp(filename, in[i]->name) == 0) {
            int index = i;

            //free node on bitmap
            im->occupied[index] = 0;

            //free the inode
            in[index]->isvalid = 0;

            //find # of data blocks occupied
            int j = 0;
            int dblocks = 0;
            while(in[index]->data_ptr[j] > 0 && j < DBLOCKS_PER_INODE) {
                dblocks++;
                j++;
            }

            //free those data blocks in dmap
            while(dblocks >= 0) {
                dm->occupied[dblocks] = 0;
                dblocks--;
            }
            return 0;
        }
    }
    perror("File not found");
    return 1;
}

int closef() {
    if(strcmp(curr_dir->name, "root") == 0) {
        perror("Can't close root directory");
        return 1;
    }

    //find parent directory
    int inode = curr_dir->parent_dir;

    //change current directory to that folder
    curr_dir = in[inode];

    puts("Folder closed.");

    return 0;
}

int readf(char *filename) {
    for(int i = 0; i < NUM_INODES; i++) {
        if(strcmp(filename, in[i]->name) == 0) {
            fseek(diskfile,blocknum*DISK_BLOCK_SIZE,SEEK_SET);

            if(fread(data,DISK_BLOCK_SIZE,1,diskfile)==1) {
                nreads++;
            } else {
                printf("ERROR: couldn't access simulated disk: %s\n",strerror(errno));
                abort();
            }
            return 0;
        }
    }
    perror("File not found");
    return 1;
}

int writef(char *filename) {
    for(int i = 0; i < NUM_INODES; i++) {
        if(strcmp(filename, in[i]->name) == 0) {

            return 0;
        }
    }
    perror("File not found");
    return 1;
}

void list() {
    int i = 0;

    //read from directory until there's nothing left
    while(curr_dir->data_ptr[i] >=0 && i < DBLOCKS_PER_INODE) {
        int inode = curr_dir->data_ptr[i];
        printf("%s", in[inode]->name);
        i++;
    }
}

int update_dir(int inode) {
    for(int i = 0; i < DBLOCKS_PER_INODE; i++) {
        if(curr_dir->data_ptr[i] < 0) {
            curr_dir->data_ptr[i] = inode;
            return 0;
        }
    }
    return 1;
}