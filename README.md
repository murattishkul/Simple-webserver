# Simple-webserver
Simple single-threaded webserver in C. Socket Programming.

All it does is:
1) Waits for client request
2) Processes the request
3) Repeats this cycle.

All of these steps are executed in a single thread.

This code imlements socket interface using socket() system call to create a socket file, binds it to host adress via bind(), that listens to ports using listen() and accept(). 

