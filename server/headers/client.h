#ifndef CLIENT_H
#define CLIENT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

typedef struct Client {
  int id;
  char *name;
  char *hostname;
  char *channel;
  int socket;
  pthread_t thread;
} Client;

typedef struct Node {
  Client *content;
  struct Node *prev;
  struct Node *next;
} Node;

Node *new_clients_stack();
Node *add_node(Node *);
Node *add_client(Node *, const char *, const char *, const char *, int);
int get_stack_length(Node *);
int print_clients(Node *);

#endif
