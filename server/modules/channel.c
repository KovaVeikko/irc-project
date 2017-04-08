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
Channel *join_client(Channel *channel, Client *client) {
  Node *new_node = add_node(channel -> clients_stack);
  new_node -> content = client;
  return channel;
}
