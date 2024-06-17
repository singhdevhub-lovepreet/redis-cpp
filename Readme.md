## A Redis implementation in C++

### This is a simple client & server communication over TCP

- Here client sends a message, server accept the connection and sends a msg back
- We have used recv() & send() sys calls, one can use read() and write() sys calls too
- because sockets are too having file descriptors and hence can be read and write into