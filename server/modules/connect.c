#include "../headers/connect.h"

void client_thread(Client *client) {
  printf("Client %s connected\n", client -> hostname);

  // receive message from client
  char received_message[MAXLINE + 1];
  ssize_t n = 1;
  
  n = read(client -> socket, received_message, MAXLINE);

  while (n > 0) {
    received_message[n] = 0;
    printf("%s: %s", client -> name, received_message);
    if(strstr(received_message, NICK) != NULL) {
      printf("changed nick to %s\n", received_message + strlen(NICK) + 1);

    }
    n = read(client -> socket, received_message, MAXLINE);
  }

};
