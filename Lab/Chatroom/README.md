> # Chatroom

## Learning Objectives

- Networking Components

## Goal

The goal of this lab is to help you understand networking components. You will accomplish this by writing a real chatroom program. You are going to write a client which can send/receive data to/from server. You will also write a server which can recieve data from multiple clients and broadcast these messages to each of the clients. The files you must modify are

- `client.c`
- `server.c`
- `questions.txt`(Not Graded)

The files `client.c` and `server.c` provide an outline of what you are expected to do via references to questions in questions.txt. For example, if you see /*QUESTION 1*/ then question 1 in `questions.txt` should help you understand what to do when filling that part of the code.
**So be sure to answer the questions in `questions.txt` to begin with**

## Client

The file `chatroom.c` is the control center for setting up the `nucurses` windows (that’s a thing) and starting up a thread for the client code.

The chatroom executable will accept three arguments: a host address, a port number, and a username.

The client program should attempt to connect to the given port at the given host address. If any argument is missing, print a helpful message and exit. Set up your connection to be TCP and IPv4. Your program will read input from the user (could be `stdin`, but we are using `ncurses` windows to make it look nice) and send it to the server it connected to. A signal interrupt will be sent to the client as a flag to tell your client to exit. To be precise, if while reading from user input your client receives a signal interrupt (`SIGINT`), free memory and close the client program. In `close_client()`, you may notice `pthread_cancel()` and `pthread_join()` in succession which is due to `ncurses`. Here you should also (as the note says) `shutdown()` the socket. Also note that we provide two functions for you to read and write to the server, you can find the functions in `utils.h`.

The chatroom executable will also create a window_object and render a view for clients to chat with each other. We will provide two functions for you to read and write messages to the window object/screen. So your job is to manage the chat window and implement `run_client()` function that sets up a connection and runs threads for writing to server and reading from server.

The figure below gives you an idea about how the client side architecture looks like:

![Alt](https://courses.engr.illinois.edu/cs241/sp2016/images/ClientArch.png)

So to sum up, your job in the client program is:

- Implement run_client()
  - Set up the network connection (TCP + IPv4).
  - Launch threads to read from the server.
  - Launch threads to write to server.
- Create a signal handler to catch interrupt and end the program gracefully.
  - Free memory you allocate.

## Server

Your server will accept a single argument: a port number. If the argument is not provided, print a helpful message and exit. Set up your server to work with TCP, IPv4, and be able to reuse ports. Your server should accept multiple client connections concurrently. Note that we want to limit the number of connections that the server handles, and this number is defined as the `MAX_CLIENTS` macro.

In addition to setting up connections, you have to process messages from clients. We have provided you with the function **processClient** to handle reading from each client. You will need to implement the code to use this in your main function. The figure below illustrates how a message propagates through the system:

![Alt](https://courses.engr.illinois.edu/cs241/sp2016/images/MessageBroadcast.png)

Lastly, your server should close gracefully when `SIGINT` is sent to the program. This means you will need to implement a signal handler. To make your life easier, create threads in a detached state so that you do not have to later join with them. You will need to use these two functions (use man pages) along with `pthread_create`: `pthread_attr_init pthread_attr_setdetachstate`

To sum up, you have to:

- Set up connections.
- Implement functionality for using `processClient()`.
- Implement a signal handler to handle a graceful shutdown.

Here is the overall client-server architecture:

![Alt](https://courses.engr.illinois.edu/cs241/sp2016/images/OverallArchitecture.png)

## Error Checking

### Networks Break!

Many things can go wrong when working with networks. Be sure to do plenty of error checking! If anything fails, print the error message and **exit(1)**. For each networking function call (except one of them) that you will be using, use **perror(NULL)**. For the one that you cannot use `perror`, use the man pages to learn how to print the error. You must figure out which function does not set `errno` upon failure and thus you cannot use `perror`.

## Testing

You could implement tests for testing functionality of client and server side code separately. That way you don’t worry about client-server interactions. You can then consider cases where a message is written to one client, is sent to the server and is then broadcasted to all the other clients running. To test such a code path, you start a test that writes to one of the clients **c** and then verifies whether all the clients have received the message that was provided as input to **c**.

Note, you are writing a server that could potentially run for a long time. This is exactly a scenario where memory leaks can be very costly and cause your server to crash. So ensure there are no memory leaks in your implementation.

Otherwise, we will leave it open ended to let you come up with other interesting test cases.

### We provided you with a Makefile.

You can run the client and server as follows: `./chatroom [address] [port] [username] ./server [port]`

Test your server and client with a friend! On your VMs you can connect to each others’ machines. Just ask them what their machine number is!

```bash
 ./server 12345
Waiting for connection...
Connection made: client_fd=4
cs241_student: hello there!
^name_sucks: more bytes
^CEnding Server
```

```bash
 ./chatroom sp16-cs241-XYZ.cs.illinois.edu 12345 steve
steve: hello there!
alec: sending bytes
^CClosing Client
```

Notice the **XYZ**, that is the machine number you will use to connect to the server (the person hosting’s machine) In the above “terminals”, there are things like “Waiting for connection…” and “Ending Server”: do not worry about having to do this. It is simply flavor text printed to the window. Feel free to do that; we will only test the bytes that are sent and received. **Because it’s particularly important for this lab, we want to reiterate that you should not have any memory leaks :)**

-------

































