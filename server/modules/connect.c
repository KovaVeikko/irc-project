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
      //printf("%s sent: \"%s\"\n", client -> name, cmd);
      if(strcmp(cmd, NICK) == 0) {
        handle_nick(client, clients_stack, strtok(NULL, " \t\r\n/"));
      }
      else if(strcmp(cmd, USER) == 0) {
        handle_user(client, strtok(NULL, " \t\r\n/"));
      }
      else if(strcmp(cmd, LIST) == 0) {
        handle_list(channels, client);
      }
      else if(strcmp(cmd, JOIN) == 0) {
        handle_join(client, strtok(NULL, " \t\r\n/"), channels);
      }
      else if(strcmp(cmd, PART) == 0) {
        handle_part(client, channels);
      }
      else if(strcmp(cmd, PRIVMSG) == 0) {
        char send_msg[MAXLINE];
        strcpy(send_msg, &received_message[9]);
        handle_privmsg(client, channels, send_msg);
        break;
      }
      else {
        send_message(client, clients_stack, received_message);
      }
      cmd = strtok(NULL, " \t\r\n/");
    }
    n = read(client -> socket, received_message, MAXLINE);
  }

};
