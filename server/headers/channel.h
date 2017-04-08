#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "client.h"

typedef struct Channel {
  char *name;
  Node *clients_stack;
} Channel;


Channel **new_channel_list();
Channel *new_channel(char *);
void add_channel(Channel **, Channel *);
void join_client(Channel *, Client *);
void part_client(Channel *, Client *);
Node *find_client_node_in_channel(Channel *, Client *);
Channel *get_or_create_channel(Channel **, char *);
char *get_channels_string(Channel **, char *);


#endif
