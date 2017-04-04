#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/client.h"

void welcome_message(Client *);
void server_message(Client *, char *);
void send_message(Client *, Node *, char *);

#endif
