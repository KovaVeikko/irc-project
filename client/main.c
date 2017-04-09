#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <time.h>

#include "./headers/utility.h"
#include "./headers/config.h"

#define MAXLINE 1000

int main(int argc , char *argv[])
{

  if(argc != 4) {
    fprintf(stderr, "Start client with command: ./main <hostname> <port> <username>\n");
    return -1;
  };

  char address[100];
  char *hostname = argv[1];
  hostname_to_ip(hostname, address);
  int port = atoi(argv[2]);
  char *username = argv[3];
  int sockfd;
  struct sockaddr_in	servaddr;

  // Create a socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return -1;
  };

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port   = htons(port);

  if (inet_pton(AF_INET, address, &servaddr.sin_addr) <= 0) {
      printf("inet_pton error for");
      return 1;
  }

  if (connect(sockfd,
              (struct sockaddr *) &servaddr,
              sizeof(servaddr)) < 0) {
      perror("connect error");
      return 1;
  }

  char connect_message[50] = "/USER ";
  strcat(connect_message, username);
  char send_message[MAXLINE];
  char server_reply[MAXLINE];
  memset(server_reply, 0, MAXLINE);

  if(write(sockfd, connect_message, strlen(connect_message)) < 0) {
    printf("%s\n", "connection message failed");
    return 1;
  }

  if(read(sockfd, server_reply, MAXLINE) < 0) {
      printf("%s\n", "connection reply failed");
      return 1;
  }
  server_reply[MAXLINE - 1] = 0;
  fputs(server_reply, stdout);



  //set up variables for select()
  fd_set all_set, r_set;
  int maxfd = sockfd + 1;
  FD_ZERO(&all_set);
  FD_SET(STDIN_FILENO, &all_set);
  FD_SET(sockfd, &all_set);
  r_set = all_set;
  struct timeval tv; tv.tv_sec = 2; tv.tv_usec = 0;

  //keep communicating with server
  for(;;){

      r_set = all_set;
      //check to see if we can read from STDIN or sock
      select(maxfd, &r_set, NULL, NULL, &tv);

      if(FD_ISSET(STDIN_FILENO, &r_set)){
        fgets(send_message, MAXLINE, stdin);
        if(write(sockfd, send_message, strlen(send_message)) < 0) {
          printf("%s\n", "send failed");
          return 1;
        }
      }

      if(FD_ISSET(sockfd, &r_set)){
        //Receive a reply from the server
        memset(server_reply, 0, MAXLINE); // täytetään puskuri nollilla
        if(read(sockfd, server_reply, MAXLINE) < 0) {
          printf("%s\n", "read failed");
        }
        server_reply[MAXLINE - 1] = 0;
        if (strcmp(server_reply, RESPONSE_OK) == 0) {
          continue;
        } else {
          fputs(server_reply, stdout);
        }
      }
  }
}
