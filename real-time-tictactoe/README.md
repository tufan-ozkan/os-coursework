# Homework 1 - Multiplayer Tic-Tac-Toe (IPC Server)

This project implements a real-time, multiplayer Tic-Tac-Toe server that communicates with player processes via bidirectional Unix pipes using `socketpair()` and handles asynchronous messages using `select()`.

## 🧠 Key Concepts
- Interprocess Communication (IPC)
- Concurrent I/O handling with `select()`
- Process forking and pipe redirection
- Real-time shared game state updates

## 🗃 Project Structure
- `server.c`: Main server implementation
- `ext2fs.h`: Provided headers for message structs and game logic
- `Makefile`: Compiles the `server` executable

## 🏁 How It Works
- Reads grid size, streak size, and player details from `stdin`
- Forks a process for each player and redirects I/O
- Handles `START` and `MARK` messages from players
- Updates game state and determines win/draw
- Uses `print_output()` to log standardized results
