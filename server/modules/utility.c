#include "../headers/utility.h"
#include "../headers/config.h"

/*
Send welcome message to client when connected
*/
void welcome_message(Client *client) {
  char *str = "\n=== Hello! Welcome to chat. ====\n\n";
  char line[2000];
  memset(line, 0, 2000);
  strcat(line, str);
  write(client -> socket, line, strlen(line));
}


/*
Inform client that the message was received successfully
*/
void response_ok(Client *client) {
  write(client -> socket, RESPONSE_OK, strlen(RESPONSE_OK));
}

/*
Send message from server to client
*/
void server_message(Client *client, char *message) {
  char *prefix = "--- ";
  char *end_char = "\n";
  char line[2000];
  memset(line, 0, 2000);
  strcpy(line, prefix);
  strcat(line, message);
  strcat(line, end_char);
  write(client -> socket, line, strlen(line));
}

/*
Send message from a client to other clients
*/
void send_message(Client *sender, Node *clients, char *message) {
  char *end_char = "";
  char *prefix = ": ";
  char line[2000];
  memset(line, 0, 2000);
  strcat(line, sender -> name);
  strcat(line, prefix);
  strcat(line, message);
  strcat(line, end_char);
  while (clients -> prev) {
    if (clients -> content -> id != sender -> id)
      write(clients -> content -> socket, line, strlen(line));
    clients = clients -> prev;
  }
}

/*
Return list of all clients in the given channel as a string
*/
char *list_clients_on_channel(Node *clients_stack, char *channel, char *str) {
  struct Node *stack = clients_stack;
  char *br = "\n";
  while (stack -> prev) {
    if (strcmp(stack -> content -> channel, channel) == 0) {
      str = realloc(str, (strlen(str) + strlen(br) + strlen(stack -> content -> name) + 1) * sizeof(char));
      strcat(str, br);
      strcat(str, stack -> content -> name);
    }
    stack = stack -> prev;
  }
  return str;
}
