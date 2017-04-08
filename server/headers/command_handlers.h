#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include "../headers/client.h"
#include "../headers/channel.h"

void handle_nick(Client *, Node *, char *);
void handle_user(Client *, char *);
void handle_list(Channel **, Client *);
void handle_join(Client *, char *, Channel **);

#endif
