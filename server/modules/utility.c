#include "../headers/utility.h"

void send_message(Client *client, char *message) {
  char *server_name = "SERVER: ";
  char *end_char = "\n";
  char *line = malloc(strlen(server_name) + strlen(message) + 2 * sizeof(char));
  strcpy(line, server_name);
  strcat(line, message);
  strcat(line, end_char);
  write(client -> socket, line, strlen(line));
}
