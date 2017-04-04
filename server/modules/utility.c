#include "../headers/utility.h"

void welcome_message(Client *client) {
  char *str = "\n=== Hello! Welcome to chat. ====\n\n";
  char *end_char = ": ";
  char *line = malloc(strlen(str) + strlen(client -> name) + strlen(end_char) + sizeof(char));
  strcat(line, str);
  strcat(line, client -> name);
  strcat(line, end_char);
  write(client -> socket, line, strlen(line));
  free(line);
}

/*
Send message from server to client
*/
void server_message(Client *client, char *message) {
  char *server_name = "SERVER: ";
  char *end_char = "\n";
  char *line = malloc(strlen(server_name) + strlen(message) + 2 * sizeof(char));
  strcpy(line, server_name);
  strcat(line, message);
  strcat(line, end_char);
  write(client -> socket, line, strlen(line));
  free(line);
}
