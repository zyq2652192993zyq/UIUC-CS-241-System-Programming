#include "chat_window.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

// This is useful
typedef struct info_t {
    char *host;
    char *port;
    char *username;
} info_t;

int serverSocket;

struct addrinfo hints, *result;
pthread_t threads[2];
void *writeVal, *readVal;

void close_client() 
{
    pthread_cancel(threads[0]);
    pthread_join(threads[0], &writeVal);
    pthread_cancel(threads[1]);
    pthread_join(threads[1], &readVal);
    close(serverSocket);
}

void *run_client(void *information) 
{
    int s;
    info_t *myinfo = (info_t *)information;
    serverSocket   = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket == -1) exit(1);

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    s = getaddrinfo(myinfo -> host, myinfo -> port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    int check = connect(serverSocket, result -> ai_addr, result -> ai_addrlen);
    if(check == -1) {
        perror("Connect");
        exit(1);
    }

    pthread_create(&threads[0], NULL, read_from_server, NULL);
    pthread_create(&threads[1], NULL, write_to_server, (myinfo -> username));

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}
