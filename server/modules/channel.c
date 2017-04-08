#include "../headers/channel.h"

/* creates an empty list of channels */
Channel **new_channel_list() {
  Channel **channels = malloc(sizeof(Channel *));
  return channels;
}

/* creates a new channel */
Channel *new_channel(char *channel_name){
  Channel *new_channel = malloc(sizeof(Channel));
  new_channel -> name = malloc((strlen(channel_name) + 1) * sizeof(char));
  strcpy(new_channel -> name, channel_name);
  new_channel -> clients_stack = malloc(0);
  return new_channel;
}

/* adds a channel to the channel list */
void add_channel(Channel **channels, Channel *channel) {
  int size = 0;
  Channel **ptr = channels;
  while (*ptr) {
    size++;
    ptr++;
  }
  channels = realloc(channels, size * sizeof(Channel *) + sizeof(Channel *));
  channels[size] = channel;
}

/* joins a client to a channel */
void join_client(Channel *channel, Client *client) {
  Node *new_node = add_node(channel -> clients_stack);
  new_node -> content = client;
  channel -> clients_stack = new_node;
  client -> channel = realloc(client -> channel, (strlen(channel -> name) + 1) * sizeof(char));
  strcpy(client -> channel, channel -> name);
}

/* finds a channel with the given name or creates one if not existing */
Channel *get_or_create_channel(Channel **channels, char *channel_name){
  while (*channels) {
    if (strcmp((*channels) -> name, channel_name) == 0) {
      return *channels;
    }
    channels++;
  }
  Channel *new = new_channel(channel_name);
  add_channel(channels, new);
  return new;
}

/* get list of channels as a string */
char *get_channels_string(Channel **channels, char *str) {
  char *br = "\n";
  char len[10 * sizeof(char)];
  while (*channels) {
    sprintf(len, " (%d)", get_stack_length((*channels) -> clients_stack));
    str = realloc(str, (strlen(str) + strlen((*channels) -> name) + strlen(len) + strlen(br) + 1) * sizeof(char));
    strcat(str, (*channels) -> name);
    strcat(str, len);
    strcat(str, br);
    channels++;
  }
  return str;
}
