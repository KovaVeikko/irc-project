#ifndef CONNECT_H
#define CONNECT_H

#include <sys/socket.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

#include "client.h"
#include "channel.h"
#include "commands.h"
#include "command_handlers.h"
#include "utility.h"
#include "config.h"

#define LISTENQ 5
#define MAXLINE 1024

Node *clients_stack;
Channel **channels;

void client_thread(Client *);

#endif
