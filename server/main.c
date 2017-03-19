// Simple IRC server

#include "headers/connect.h"

int main(int argc, char **argv) {
    int listenfd, connfd = -1;
    socklen_t cliaddr_size;
    struct sockaddr_in6	servaddr, cliaddr;

    if(argc != 2) {
      fprintf(stderr, "Start server with command: ./main <port>\n");
      return -1;
    };

    // Create a master socket
    if((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      return -1;
    };

    // Choose port to listen to and accept connections from any address
    bzero(&servaddr, sizeof(servaddr));
  	servaddr.sin6_family    = AF_INET6;
  	servaddr.sin6_addr	= in6addr_any;
  	servaddr.sin6_port      = htons(atoi(argv[1]));

    // bind() the socket to localhost (given port)
    if (bind(listenfd, (struct sockaddr *) &servaddr,
  		 sizeof(servaddr)) < 0) {
  		perror("bind");
  		return -1;
  	}

    // listen() max LISTENQ connections
    if(listen(listenfd, LISTENQ) == -1) {
        perror("listen");
        return -1;
    };

    printf("Server listening port %s.\n", argv[1]);
    printf("Press CTRL + C to exit.\n");

    // initialise an empty list of clients
    Node *clients_stack = new_clients_stack();

    // wait connections forever...
    for ( ; ; ) {

      // save client address and port into cliaddr struct
      if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr,
  				     &cliaddr_size)) < 0) {
  		    perror("accept");
  		    return -1;
  		}

      // add connection to stack
      char buff[80];
      clients_stack = add_client(
        clients_stack,
        "dummy name",
        inet_ntop(AF_INET6, &cliaddr.sin6_addr, buff, sizeof(buff)),
        connfd
      );

      // receive messages from and respond to the connected client in a new thread
      pthread_create(&(clients_stack -> content -> thread), NULL, (void * (*) (void *)) client_thread, clients_stack -> content);
    }



};
