#include "../headers/client.h"
#include "../headers/channel.h"
#include "../headers/utility.h"
#include "../headers/command_handlers.h"
#include "../headers/config.h"

char CHANNELS[][20] = {"Cellari", "HappyGarden"};

// Channel CHANNELS2[][sizeof(Channel)];

void handle_user(Client *client, char *username) {
    if (username == NULL) {
      char *error = "ERROR: No username parameter given";
      server_message(client, error);
      return;
    }
    client -> name = realloc(client -> name, (strlen(username) + 1) * sizeof(char));
    strcpy(client -> name, username);
    server_message(client, CONNECT_MESSAGE);
};

void handle_nick(Client *client, Node *clients, char *name) {
  if (name == NULL) {
    char *error = "ERROR: No name parameter given";
    server_message(client, error);
    return;
  }
  char *str = "You are now known as ";
  char *message = malloc(strlen(str) + strlen(name) + sizeof(char));
  strcat(message, str);
  strcat(message, name);
  server_message(client, message);
  free(message);
  client -> name = realloc(client -> name, (strlen(name) + 1) * sizeof(char));
  strcpy(client -> name, name);
}

void handle_list(Client *client) {
  char *str = "Channels: \n";
  char *br = "\n";
  char *message = malloc(2048);
  memset(message, 0, 2048);
  strcat(message, str);
  for(int i = 0; i < sizeof(CHANNELS) / sizeof(CHANNELS[0]); i++) {
    strcat(message, CHANNELS[i]);
    strcat(message, br);
  }
  server_message(client, message);
  free(message);
};

void handle_join(Client *client, Node *clients, char *channel) {
  if (channel == NULL) {
    char *error = "ERROR: No channel parameter given";
    server_message(client, error);
    return;
  }
  if (strlen(channel) > 20) {
    char *error = "ERROR: Channel name too long. Max 20 char.";
    server_message(client, error);
    return;
  }
  char *str = "You are now on channel ";
  char *mbr = ". Members are:";
  char *message = malloc(strlen(str) + (strlen(channel) + strlen(mbr) + 1) * sizeof(char));
  strcat(message, str);
  strcat(message, channel);
  strcat(message, mbr);
  client -> channel = realloc(client -> channel, (strlen(channel) + 1) * sizeof(char));
  strcpy(client -> channel, channel);
  message = list_clients_on_channel(clients, channel, message);
  server_message(client, message);
  free(message);
}
