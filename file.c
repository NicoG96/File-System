#include "fs.h"

void mkdir(char *filename) {
    //find free inode
    int free_node = find_free_inode(im);

    /* each index in bitmap corresponds to an inode
     * get base address of inodes, add it to ith inode */
    int dblock = find_free_datblock(dm);
    int dataAddr = (dblock * BLOCK_SIZE + sb->data);

    //initialize inode
    inode_create(filename, dataAddr, free_node, 1);

    //update directory list with new file
    if(update_dir(free_node)) {
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
    inode_create(filename, dataAddr, free_node, 0);

    //update directory list with new file
    if(strcmp(filename, "root") != 0) {
        if(update_dir(free_node)) {
            perror("Directory full");
        } else {
            printf("File '%s' successfully created\n", filename);
        }
    }
}

int openf(char *filename) {
    for(int i = 0; i < NUM_INODES; i++) {
        if(strcmp(filename, in[i]->name) == 0) {
            if(in[i]->isdir) {
                curr_dir = in[i];
                printf("Opened '%s'\n", filename);
                //printf("%s\n", curr_dir->name);
                return 0;
            }
            else {
                perror("File is not a directory");
            }

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
            FILE *filesys = fdopen(fs, "w+");
            char buf[BLOCK_SIZE];
            inode *read = in[i];
            int j = 0;

            //count the data blocks it occupies
            while(read->data_ptr[j] >= 0 && j < DBLOCKS_PER_INODE) {
                fseek(filesys, read->data_ptr[j], SEEK_SET);

                fread(buf, BLOCK_SIZE, 1, filesys);
                printf("%s", buf);
                j++;
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
            char buf[BLOCK_SIZE];
            FILE *filesys = fdopen(fs, "w+");
            inode *read = in[i];

            //get user input
            puts("Please enter data to write:");
            fgets(buf, 512, stdin);
            //printf("%s\n", buf);

            fseek(filesys, read->data_ptr[0], SEEK_SET);
            fwrite(buf, BLOCK_SIZE, 1, filesys);

            /* TEST */
            /*
            char out[BLOCK_SIZE];
            fseek(filesys, read->data_ptr[0], SEEK_SET);
            fread(out, BLOCK_SIZE, 1, filesys);
            printf("%s\n", out);
            */

            fsync(fs);
            puts("Successful write.");
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