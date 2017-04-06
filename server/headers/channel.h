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


#endif
