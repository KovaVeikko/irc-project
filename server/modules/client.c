#include "../headers/client.h"

/* Initialises a stack */
Node *new_clients_stack() {
  Node *stack = malloc(sizeof(Node));
  stack -> content = malloc(sizeof(Client));
  stack -> content -> id = -1;
  stack -> prev = NULL;
  stack -> next = NULL;
  return stack;
};

/* Adds a new Node to the stack */
Node *add_client(Node *stack, const char *name, const char *hostname, const char *channel, int socket) {
  Node *new_node = malloc(sizeof(Node));
  new_node -> next = NULL;
  new_node -> prev = stack;
  stack -> next = new_node;
  Client *client = malloc(sizeof(Client));
  client -> id = stack -> content -> id + 1;
  client -> name = malloc(strlen(name) * sizeof(char) + 1);
  strcpy(client -> name, name);
  client -> hostname = malloc(strlen(hostname) * sizeof(char) + 1);
  strcpy(client -> hostname, hostname);
  client -> channel = malloc(strlen(channel) * sizeof(char) + 1);
  strcpy(client -> channel, channel);
  client -> socket = socket;
  new_node -> content = client;
  return new_node;
};

/* list all connected clients */
int print_clients(Node *stack) {
  if (stack == NULL) {
    return -1;
  }
  printf("%-5s%-15s%-25s%-25s%-5s\n", "Id", "Name", "Hostname", "Channel", "Socket");
  while (stack -> prev) {
    printf("%-5d%-15s%-25s%-25s%-5d\n",
      stack -> content -> id,
      stack -> content -> name,
      stack -> content -> hostname,
      stack -> content -> channel,
      stack -> content -> socket
    );
    stack = stack -> prev;
  }
  printf("\n");
  return 0;
}
