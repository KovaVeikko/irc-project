// Simple IRC server

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

#define LISTENQ 5

int main(int argc, char **argv) {
    int listenfd, connfd = -1;
    socklen_t cliaddr_size;
    struct sockaddr_in6	servaddr, cliaddr;

    if(argc != 2) {
      fprintf(stderr, "Start server with command: ./main <port>\n");
      return -1;
    };

    // Create socket
    if((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      return -1;
    };

    // Choose port to listen and accept connections from any address
    bzero(&servaddr, sizeof(servaddr));
  	servaddr.sin6_family    = AF_INET6;
  	servaddr.sin6_addr	= in6addr_any;
  	servaddr.sin6_port      = htons(atoi(argv[1]));

    // bind() local address
    if (bind(listenfd, (struct sockaddr *) &servaddr,
  		 sizeof(servaddr)) < 0) {
  		perror("bind");
  		return -1;
  	}

    // listen() connections from the world
    if(listen(listenfd, LISTENQ) == -1) {
        perror("listen");
        return -1;
    };

    printf("Server listening port %s.\n", argv[1]);
    printf("Press CTRL + C to exit.\n");

    // wait connections forever
    for ( ; ; ) {

      // Odotetaan sisääntulevaa yhteyttä, ei pitäisi
  		// blokata, koska FD_ISSET
  		// Yhteydenottajan IP-osoite ja portti otetaan talteen
  		// cliaddr-rakenteeseen
      if ((connfd = accept(listenfd, (struct sockaddr *) &cliaddr,
  				     &cliaddr_size)) < 0) {
  		    perror("accept");
  		    return -1;
  		}

      // print client address and port
      char buff[80];
      printf("connection from %s, port %d\n",
  		       inet_ntop(AF_INET6, &cliaddr.sin6_addr,
  				 buff, sizeof(buff)),
  		       ntohs(cliaddr.sin6_port));


    }

};
