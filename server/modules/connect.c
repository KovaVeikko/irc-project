#include "../headers/connect.h"

void client_thread(Client *client) {
  printf("Client %s connected\n", client -> hostname);

  // receive message from client
  char received_message[MAXLINE + 1];
  ssize_t n = 1;

  received_message[n] = 0;
  printf("%s", received_message);

  while (n > 0) {
    received_message[n] = 0;
    printf("%s\n", received_message);
    n = read(client -> socket, received_message, MAXLINE);

  }

};
