# Homework 3 - EXT2 File System Chronology (`histext2fs`)

This project implements a tool that reconstructs the file and directory hierarchy and recovers the historical actions (like `touch`, `mkdir`, `mv`, `rm`, `rmdir`) performed on an EXT2 file system image.

## 🧠 Key Concepts
- EXT2 file system internals
- Inodes, data blocks, indirect block resolution
- Timestamps (access, change, modify, delete)
- Ghost entries, forensic recovery
- Tree traversal and path reconstruction

## ⚙️ Features
- Parses and traverses EXT2 file system image
- Displays full current file/directory hierarchy
- Identifies and displays ghost entries
- Reconstructs action history with timestamps and arguments

## 🧪 Sample Usage
```bash
./histext2fs image.img state_output.txt history_output.txt
