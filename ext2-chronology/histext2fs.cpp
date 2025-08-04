#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "ext2fs.h"

struct ext2_super_block sb;
struct ext2_block_group_descriptor *bgds;

int fileImage;
int blockSize, inodeSize, blocksPerGroup, inodesPerGroup;
int numBlockGroups;

void readBlock(int blockNum, void *buffer) {
    lseek(fileImage, blockNum * blockSize, SEEK_SET);
    read(fileImage, buffer, blockSize);
}

void readInode(int inodeNum, struct ext2_inode *inode) {
    int groupIndex = (inodeNum - 1) / inodesPerGroup;
    int localIndex = (inodeNum - 1) % inodesPerGroup;
    int inodeTable = bgds[groupIndex].inode_table;
    int inodeOffset = inodeTable * blockSize + localIndex * inodeSize;

    lseek(fileImage, inodeOffset, SEEK_SET);
    read(fileImage, inode, inodeSize);
}

int isInodeAlloc(int inodeNum) {
    int groupIndex = (inodeNum - 1) / inodesPerGroup;
    int localIndex = (inodeNum - 1) % inodesPerGroup;
    int bitmapBlock = bgds[groupIndex].inode_bitmap;

    uint8_t *bitmap = (uint8_t *)malloc(blockSize);
    readBlock(bitmapBlock, bitmap);

    int alloc = (bitmap[localIndex / 8] >> (localIndex % 8)) & 1;

    free(bitmap);
    return alloc;
}

void printCurrState(int inodeNum, int depth, FILE *out) {
    struct ext2_inode inode;
    readInode(inodeNum, &inode);

    int size = inode.size;
    int bytes = 0;
    
    for (int i = 0; i < EXT2_NUM_DIRECT_BLOCKS && bytes < size; ++i) {
        if (!inode.direct_blocks[i]) continue;
                
        uint8_t *block = (uint8_t *)malloc(blockSize);
        readBlock(inode.direct_blocks[i], block);

        int remaining = (size - bytes < blockSize) ? (size - bytes) : blockSize;
        
        int pos = 0;
        while (pos < remaining) {
            struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + pos);
            if (entry->length == 0) break;

            if (strcmp(entry->name, ".") != 0 && strcmp(entry->name, "..") != 0 && entry->inode != 0) {
                for (int j = 0; j < depth; j++) fprintf(out,"-");

                if (isInodeAlloc(entry->inode)) {
                    struct ext2_inode child;
                    readInode(entry->inode, &child);

                    int is_dir = EXT2_S_ISDIR(child.mode);
                    fprintf(out, " %d:%s%s\n", entry->inode, entry->name, is_dir ? "/" : "");
                    if (is_dir) {
                        printCurrState(entry->inode, depth + 1, out);
                    }
                }
            }
            pos += entry->length;
        }

        pos = 0;
        while (pos < remaining) {
            struct ext2_dir_entry *entry = (struct ext2_dir_entry *)(block + pos);
            if (entry->length == 0) break;

            int expectedLength = sizeof(struct ext2_dir_entry) + entry->name_length;
            expectedLength = (expectedLength + 3) & ~3; // align to 4 bytes

            if (entry->length > expectedLength + 8) {
                int ghost_pos = pos + expectedLength;

                while (ghost_pos + sizeof(struct ext2_dir_entry) < pos + entry->length) {
                    struct ext2_dir_entry *ghost = (struct ext2_dir_entry *)(block + ghost_pos);
                    if (ghost->inode == 0 || ghost->name_length == 0 || ghost->name_length > 255) break;

                    char ghostName[256] = {0};
                    memcpy(ghostName, ghost->name, ghost->name_length);
                    ghostName[ghost->name_length] = '\0';

                    if (strcmp(ghostName, ".") != 0 && strcmp(ghostName, "..") != 0) {
                        for (int j = 0; j < depth; j++) fprintf(out, "-");
                        fprintf(out, " (%d:%s)\n", ghost->inode, ghostName);
                    }

                    int ghost_len = sizeof(struct ext2_dir_entry) + ghost->name_length;
                    ghost_len = (ghost_len + 3) & ~3;

                    ghost_pos += ghost_len;
                }
            }
            pos += entry->length;
        }
        free(block);
        bytes += blockSize;
    }
}

int main(int argc, char *argv[]) {
    
    fileImage = open(argv[1], O_RDONLY);

    lseek(fileImage, EXT2_SUPER_BLOCK_SIZE, SEEK_SET);
    read(fileImage, &sb, sizeof(sb));
    
    blockSize = EXT2_UNLOG(sb.log_block_size);
    inodeSize = sb.inode_size;
    blocksPerGroup = sb.blocks_per_group;
    inodesPerGroup = sb.inodes_per_group;
    numBlockGroups = (sb.block_count + blocksPerGroup - 1) / blocksPerGroup;

    int bgd_block = (blockSize == 1024) ? 2 : 1;
    bgds = (ext2_block_group_descriptor *)malloc(sizeof(*bgds) * numBlockGroups);

    lseek(fileImage, bgd_block * blockSize, SEEK_SET);
    read(fileImage, bgds, sizeof(struct ext2_block_group_descriptor) * numBlockGroups);

    FILE *out = fopen(argv[2], "w");

    fprintf(out, "- %d:root/\n", EXT2_ROOT_INODE);
    printCurrState(EXT2_ROOT_INODE, 2, out);

    fclose(out);
    free(bgds);
    close(fileImage);
    return 0;
}
