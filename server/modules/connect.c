#include "../headers/connect.h"

void client_thread(Client *client) {
  printf("Client %s connected\n", client -> hostname);

  // receive message from client
  char received_message[MAXLINE + 1];
  char *msg = malloc(MAXLINE + 1);
  char *cmd;
  ssize_t n = 1;

  n = read(client -> socket, received_message, MAXLINE);

  while (n > 0) {
    received_message[n] = 0;
    msg = strcpy(msg, received_message);
    cmd = strtok(msg, " \t\r\n/");
    while(cmd != NULL) {
      printf("%s sent: \"%s\"\n", client -> name, cmd);
      if(strcmp(cmd, NICK) == 0) {
        handle_nick(client, clients_stack, strtok(NULL, " \t\r\n/"));
      }
      if(strcmp(cmd, USER) == 0) {
        handle_user(client);
      }
      if(strcmp(cmd, LIST) == 0) {
        handle_list(client);
      }
      else {
        send_message(client, clients_stack, received_message);
      }
      cmd = strtok(NULL, " \t\r\n/");
    }
    n = read(client -> socket, received_message, MAXLINE);
  }

};
