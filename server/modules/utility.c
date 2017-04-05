#include "../headers/utility.h"

/*
Send welcome message to client when connected
*/
void welcome_message(Client *client) {
  char *str = "\n=== Hello! Welcome to chat. ====\n\n";
  char *line = malloc(strlen(str) + sizeof(char));
  strcat(line, str);
  write(client -> socket, line, strlen(line));
  free(line);
}

/*
Send message from server to client
*/
void server_message(Client *client, char *message) {
  char *prefix = "--- ";
  char *end_char = "\n";
  char *line = malloc(strlen(prefix) + strlen(message) + 2 * sizeof(char));
  strcpy(line, prefix);
  strcat(line, message);
  strcat(line, end_char);
  write(client -> socket, line, strlen(line));
  free(line);
}

/*
Send message from a client to other clients
*/
void send_message(Client *sender, Node *clients, char *message) {
  char *end_char = "";
  char *prefix = ": ";
  char *line = malloc(strlen(sender -> name) + strlen(prefix) + strlen(message) + strlen(end_char) + sizeof(char));
  strcat(line, sender -> name);
  strcat(line, prefix);
  strcat(line, message);
  strcat(line, end_char);
  while (clients -> prev) {
    if (clients -> content -> id != sender -> id)
      write(clients -> content -> socket, line, strlen(line));
    clients = clients -> prev;
  }
  free(line);
}
