#include "../headers/client.h"
#include "../headers/utility.h"
#include "../headers/command_handlers.h"

void handle_nick(Client *client, Node *clients, char *name) {
  if (name == NULL) {
    char *error = "No new name given";
    server_message(client, error);
    return;
  }
  strcpy(client -> name, name);
  char *str = "You are now known as ";
  char *message = malloc(strlen(str) + strlen(name) + sizeof(char));
  strcat(message, str);
  strcat(message, name);
  server_message(client, message);
  free(message);
}
