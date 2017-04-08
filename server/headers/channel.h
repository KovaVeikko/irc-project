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
Channel *join_client(Channel *, Client *);


#endif
