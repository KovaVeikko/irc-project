#include "../headers/connect.h"

void connect_client(Client *client) {
  printf("Client %s connected\n", client -> hostname);

  // receive message from client
  char received_message[MAXLINE + 1];
  ssize_t n;
  n = read(client->socket, received_message, MAXLINE);

  while (n > 0) {
    received_message[n] = 0;
    printf("%s\n", received_message);
    n = read(client->socket, received_message, MAXLINE);
  }

};
