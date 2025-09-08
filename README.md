
# Client-Server Terminal-Based Chat Application in C

## Overview
This is a simple terminal-based chat application written in C that demonstrates basic client-server communication using TCP sockets. Multiple clients can connect to the server and exchange messages in real time. The server handles multiple clients simultaneously using the `select()` system call.

## Features
- Multiple clients can connect and chat concurrently.
- Real-time message broadcasting from clients to all other connected clients.
- Simple terminal-based user interface.
- Basic error handling and connection management.

## Technologies Used
- C Programming Language
- TCP Sockets
- select() system call for I/O multiplexing
- Linux Terminal

## How to Run

### 1. Compile Server and Client
```bash
gcc server.c -o server
gcc client.c -o client
```

### 2. Start the Server
```bash
./server <PORT>
```
Example:
```bash
./server 8080
```

### 3. Start a Client
```bash
./client <SERVER_IP> <PORT>
```
Example:
```bash
./client 127.0.0.1 8080
```

### 4. Chat
Once connected, type messages in the client terminal and they will be broadcast to all connected clients.

## Folder Structure
```
.
├── server.c         # Server source code
├── client.c         # Client source code
├── README.md        # Project description
└── Makefile         # Optional: for easy compilation
```

## Notes
- Make sure the server is started before any clients attempt to connect.
- The application works on Unix-like systems (Linux, macOS).
- Error handling is minimal for learning purposes.

## Author
Rishabh Jain

## License
MIT License
