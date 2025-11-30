Compilation
This project requires a GCC compiler and a Linux/Unix environment(or WSL on Windows).

**Compile the Server:**
gcc server.c -o server
**Compile the Client:**
gcc client.c -o client

You must start the server first, followed by the client.
You will need two separate terminal windows (one for the server, one for the client).

The server requires a port number as an argument.
# Example:
./server 5000

The client requires the Server IP and Port number. If running locally, use 127.0.0.1.
# Example:
./client 127.0.0.1 5000

Type the message in the client window to check, the server gets the message.
The server will display the message back.

Type "EXIT" to close the connection .

Assumption made :
The buffer size is set to 256 bytes. Messages longer than this will be split.
Protocol: Uses IPv4 (AF_INET) and TCP (SOCK_STREAM).
We assumed that one send() from the client equals exactly one recv() on the server.
we kept queue on the server up to 5 beyond that it wont stay in the queue.
