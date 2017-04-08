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
Node *add_node(Node *stack) {
  Node *new_node = malloc(sizeof(Node));
  new_node -> next = NULL;
  new_node -> prev = stack;
  stack -> next = new_node;
  return new_node;
}

/* Removes Node from the stack */
Node *remove_node(Node *node) {
  Node *prev = node -> prev;
  Node *next = node -> next;
  Node *last;
  if (prev != NULL) {
    node -> prev -> next = node -> next;
    last = node -> prev;
  }
  if (next != NULL) {
    last = node -> next;
    node -> next -> prev = node -> prev;
  }
  free(node);
  return last;
}

/* Creates a new Client and adds to the stack */
Node *add_client(Node *stack, const char *name, const char *hostname, const char *channel, int socket) {
  Node *new_node = add_node(stack);
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

int get_stack_length(Node *stack) {
  int n = 0;
  while (stack -> prev) {
    n++;
    stack = stack -> prev;
  }
  return n;
}

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
