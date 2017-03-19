#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Client {
  int id;
  char *name;
  int socket;
  char *hostname;
} Client;

typedef struct Node {
  Client *content;
  struct Node *prev;
  struct Node *next;
} Node;

Node *new_clients_stack();
Node *add_client(Node *, const char *, const char *, int);
int print_clients(Node *);

#endif
