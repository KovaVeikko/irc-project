#include "../headers/channel.h"
#include "../headers/config.h"


/* creates an empty list of channels */
Channel **new_channel_list() {
  Channel **channels = malloc(sizeof(Channel *) * MAX_CHANNELS);
  memset(channels, 0, sizeof(Channel *) * 1000);
  channels[0] = NULL;
  return channels;
}

/* creates a new channel */
Channel *new_channel(char *channel_name) {
  Channel *new = malloc(sizeof(Channel));
  new -> name = malloc((strlen(channel_name) + 1) * sizeof(char));
  strcpy(new -> name, channel_name);
  new -> clients_stack = malloc(0);
  return new;
}

/* adds a channel to the channel list */
void add_channel(Channel **channels, Channel *channel) {
  int size = 0;
  while (*(channels + size) != NULL) {
    size++;
  }
  channels[size] = channel;
  channels[size + 1] = NULL;
}

/* signs a client into a channel */
void join_client(Channel *channel, Client *client) {
  Node *new_node = add_node(channel -> clients_stack);
  new_node -> content = client;
  channel -> clients_stack = new_node;
  client -> channel = realloc(client -> channel, (strlen(channel -> name) + 1) * sizeof(char));
  strcpy(client -> channel, channel -> name);
}

/* finds a client Node from channel's client stack and returns it if found */
Node *find_client_node_in_channel(Channel *channel, Client *client) {
  Node *node = channel -> clients_stack;
  while (node -> prev) {
    if (node -> content -> id == client -> id) {
      return node;
    }
    node = node -> prev;
  }
  return NULL;
}

/* resigns a client from a channel */
void part_client(Channel *channel, Client *client) {
  Node *node = find_client_node_in_channel(channel, client);
  if (node != NULL) {
    channel -> clients_stack = remove_node(node);
    client -> channel = realloc(client -> channel, (strlen(DEFAULT_CHANNEL) + 1) * sizeof(char));
    strcpy(client -> channel, DEFAULT_CHANNEL);
  }
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
  Channel **ptr = channels;
  while (*ptr) {
    sprintf(len, " (%d)", get_stack_length((*ptr) -> clients_stack));
    str = realloc(str, (strlen(str) + strlen((*ptr) -> name) + strlen(len) + strlen(br) + 1) * sizeof(char));
    strcat(str, (*ptr) -> name);
    strcat(str, len);
    strcat(str, br);
    ptr++;
  }
  return str;
}

/* get list of members on a channel as a string */
char *get_members_string(Channel *channel, char *str) {
  char *prefix = "members are:\n";
  char *br = "\n";
  Node *node = channel -> clients_stack;
  str = realloc(str, (strlen(prefix) + 1) * sizeof(char));
  strcpy(str, prefix);
  while (node -> prev) {
    str = realloc(str, (strlen(str) + strlen(br) + strlen(node -> content -> name) + 1) * sizeof(char));
    strcat(str, node -> content -> name);
    strcat(str, br);
    node = node -> prev;
  }
  return str;
}
