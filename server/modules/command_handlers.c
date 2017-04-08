#include "../headers/client.h"
#include "../headers/channel.h"
#include "../headers/utility.h"
#include "../headers/command_handlers.h"
#include "../headers/config.h"

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

void handle_list(Channel **channels, Client *client) {
  char *str = "Channels: \n";
  char *message = malloc((strlen(str) + 1) * sizeof(char));
  strcat(message, str);
  message = get_channels_string(channels, message);
  server_message(client, message);
  free(message);
};

void handle_join(Client *client, char *channel_name, Channel **channels) {
  if (channel_name == NULL) {
    char *error = "ERROR: No channel parameter given";
    server_message(client, error);
    return;
  }
  if (strlen(channel_name) > 20) {
    char *error = "ERROR: Channel name too long. Max 20 char.";
    server_message(client, error);
    return;
  }
  if (strcmp(client -> channel, channel_name) == 0) {
    char *msg = "You are already on that channel.";
    server_message(client, msg);
    return;
  }
  if (strcmp(client -> channel, DEFAULT_CHANNEL) != 0) {
    Channel *prev_channel = get_or_create_channel(channels, client -> channel);
    part_client(prev_channel, client);
  }
  Channel *channel = get_or_create_channel(channels, channel_name);
  join_client(channel, client);
  char *message = malloc(0);
  char *str = "You are now on channel ";
  message = realloc(message, (strlen(str) + strlen(channel -> name) + 1) * sizeof(char));
  strcat(message, str);
  strcat(message, channel -> name);
  server_message(client, message);
  free(message);
  char *notification = "joined channel\n";
  send_message(client, channel -> clients_stack, notification);
}

void handle_part(Client *client, Channel **channels) {
  if (strcmp(client -> channel, DEFAULT_CHANNEL) == 0) {
    char *msg = "You have not joined any channel.";
    server_message(client, msg);
    return;
  }
  Channel *channel = get_or_create_channel(channels, client -> channel);
  part_client(channel, client);
  char *notification = "left channel\n";
  send_message(client, channel -> clients_stack, notification);
}

void handle_privmsg(Client *client, Channel **channels, char *message) {
  if (strcmp(client -> channel, DEFAULT_CHANNEL) == 0) {
    char *msg = "You have not joined any channel.";
    server_message(client, msg);
    return;
  }
  Channel *channel = get_or_create_channel(channels, client -> channel);
  send_message(client, channel -> clients_stack, message);
}
