#include "../headers/command_handlers.h"
#include "../headers/commands.h"

void handle_nick(Client *client, Node *clients, char *name) {
  strcpy(client -> name, name);
  printf("%s changed nick to %s\n", client -> name, name);
}
