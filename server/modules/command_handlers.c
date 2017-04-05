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
    char *error = "ERROR: No name parameter given";
    server_message(client, error);
    return;
  }
  client -> name = realloc(client -> name, (strlen(name) + 1) * sizeof(char));
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
  char *message = malloc(2048);
  strcat(message, str);
  for(int i = 0; i < sizeof(CHANNELS) / sizeof(CHANNELS[0]); i++) {
    strcat(message, CHANNELS[i]);
    strcat(message, br);
  }
  strcat(message, end);
  server_message(client, message);
  free(message);
};

void handle_join(Client *client, char *channel) {
  if (channel == NULL) {
    char *error = "ERROR: No channel parameter given";
    server_message(client, error);
    return;
  }
  char *str = "You are now on channel ";
  char *message = malloc(strlen(str) + (strlen(channel) + 1) * sizeof(char));
  strcat(message, str);
  strcat(message, channel);
  client -> channel = realloc(client -> channel, (strlen(channel) + 1) * sizeof(char));
  strcpy(client -> channel, channel);
  server_message(client, message);
  free(message);
}
