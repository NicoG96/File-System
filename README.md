# File-System
Designed a file system mapping using block allocation
___

Casted mmap calls to structs representing logical sections of the filesystem. These sections included most saliently: the superblock, inode bitmap block, data bitmap block, inode blocks, and finally the raw data blocks.

The filesystem would then initialize these blocks and store the pertinent data within the superblock.  The program then uses this block as a logical reference for all other blocks on the disk.  Specific sections of a file could be located through logical manipulations of memory locations.  Parsing a file, for instance, involves locating the raw data in memory by indexing block 0 of the data section and adding the offset stored within the file's inode to that memory location. Likewise, files can be contiguously read by updating this offset by the amount of bytes written/read.

This entire program is contained within a single file and is meant to support lightweight filesystem functions. As of current writing, the program supports the ability to open/close folders, create files, read/write to files, and creating directories.
