#include "server.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

void *processClient(void *arg);
void write_to_clients(const char *message, int len);

int endingServer;
int serverSocket;
int clients[MAX_CLIENTS];
int clientsConnected;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void close_server() 
{
    // Use a signal handler to call this function and close the server
    close(serverSocket);
    pthread_mutex_destroy(&mutex);
    exit(1);
}

int main(int argc, char **argv) 
{
    signal(SIGINT, close_server);

    if(argc != 2){
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    int s;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    int myval = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &myval, sizeof(myval));

    if (bind(serverSocket, result -> ai_addr, result -> ai_addrlen) != 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(serverSocket, MAX_CLIENTS) != 0) {
        perror("listen()");
        exit(1);
    }

    printf("Waiting for connection...\n");
    int client_fd;
    while(1) {
        client_fd = accept(serverSocket, NULL, NULL);
        if(clientsConnected >= MAX_CLIENTS){
            close(client_fd);
            continue;
        }

        printf("Connection made: client_fd = %d\n", client_fd);
        pthread_t thread1;
        pthread_attr_t tattr;
        pthread_attr_init(&tattr);
        pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);

        if(client_fd != -1){
            pthread_mutex_lock(&mutex);
            clients[clientsConnected] = (intptr_t)client_fd;
            ++clientsConnected;
            pthread_create(&thread1, 0, processClient, (void*)((intptr_t)client_fd));
            pthread_mutex_unlock(&mutex);
        }
    }

    return 0;
}

// DO NOT MODIFY THIS FUNCTION
void write_to_clients(const char *message, int len) 
{
    for (int i = 0; i < clientsConnected; ++i) {
        write(clients[i], message, len);
    }
}

// DO NOT MODIFY THIS FUNCTION (BUT DO USE IT)
void *processClient(void *arg) 
{
    int client_fd           = (intptr_t)arg;
    int client_is_connected = 1;

    while (client_is_connected) {
        char buffer[MSG_SIZE];
        int len = 0;
        int num;

        // Read until client sends eof or \n is read
        while (1) {
            num = read(client_fd, buffer + len, MSG_SIZE);
            len += num;

            if (!num) {
                client_is_connected = 0;
                break;
            }
            if (buffer[len - 1] == '\n') break;
        }

        // Error or client closed the connection, so time to close this specific
        // client connection
        if (!client_is_connected) {
            printf("User %d left\n", client_fd);
            break;
        }

        write_to_clients(buffer, len);
    }

    close(client_fd);
    pthread_mutex_lock(&mutex);
    --clientsConnected;
    pthread_mutex_unlock(&mutex);

    return NULL;
}
