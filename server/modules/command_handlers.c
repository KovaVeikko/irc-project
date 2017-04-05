#include "../headers/client.h"
#include "../headers/utility.h"
#include "../headers/command_handlers.h"

char *CHANNELS[] = {"Cellari", "HappyGarden"};

void handle_user(Client *client) {
    char *message = "Hello! Welcome to chat.";
    server_message(client, message);
};

void handle_nick(Client *client, Node *clients, char *name) {
  if (name == NULL) {
    char *error = "No name parameter given";
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

void handle_list(Client *client) {
  char *str = "Channels -> \n";
  char *end = "<- End of list";
  char *br = "\n";
  char message[1024];
  strcat(message, str);
  for(int i = 0; i < sizeof(CHANNELS) / sizeof(CHANNELS[0]); i++) {
    strcat(message, CHANNELS[i]);
    strcat(message, br);
  }
  strcat(message, end);
  server_message(client, message);
};
