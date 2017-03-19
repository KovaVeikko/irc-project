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

#define LISTENQ 5
#define MAXLINE 1024

void client_thread(Client *);

#endif
