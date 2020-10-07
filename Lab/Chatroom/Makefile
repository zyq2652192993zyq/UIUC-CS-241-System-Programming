CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -O2 -std=gnu11
LFLAGS=-lpthread -lncurses

ALL : chatroom server

chatroom : chatroom.o chat_window.o client.o utils.o
	$(CC) $(CFLAGS) chatroom.o chat_window.o client.o utils.o -o chatroom $(LFLAGS)

server : server.o
	$(CC) $(CFLAGS) server.o -o server $(LFLAGS)

chatroom.o : chatroom.c
	$(CC) -c chatroom.c

chat_window.o : chat_window.c
	$(CC) -c chat_window.c

client.o : client.c
	$(CC) -c client.c

utils.o : utils.c
	$(CC) -c utils.c

server.o : server.c
	$(CC) -c server.c

.PHONY: clean

clean:
	rm *.o chatroom server



