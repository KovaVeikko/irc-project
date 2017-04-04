/* TCP Client for IRC  */
#include <sys/socket.h>  // defines socket, connect, ...
#include <netinet/in.h>  // defines sockaddr_in
#include <string.h>      // defines memset
#include <stdio.h>       // defines printf, perror, ...
#include <arpa/inet.h>   // inet_pton, ...
#include <unistd.h>      // read, ...
#include <stdlib.h>

#include <sys/types.h>
#include <netdb.h>


#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char **argv) {
    // define variables
    int sockfd, portno, n;
    char buf[BUFSIZE];
    struct sockaddr_in servaddr; // tietorakenne, joka esittää osoitetta

    const char *address = "127.0.0.1";
    //portno = 9000;

    // Luodaan pistoke, joka käyttää IPv4 - protokollaa (AF_INET)
    // ja TCP-protokollaa (SOCK_STREAM)
    // Paluuarvo on pistokkeen tunniste, tai -1 jos luominen ei onnistunut
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
        error("Socket error");
        return 1;
    }

    // Alustetaan osoitetta esittävä tietorakenne nollilla.
    // Sen jälkeen kerrotaan että osoiteperhe on IPv4,
    // ja määritellään palvelimen portti johon tullaan ottamaan yhteyttä
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(9000);


    // Muutetaan ASCII-muotoinen IP-osoitte binääriseksi.
    // Se talletetaan servaddr - rakenteeseen.
    if (inet_pton(AF_INET, address, &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        return 1;
    }

    //TCP yhteys
    if (connect(sockfd,
                (struct sockaddr *) &servaddr,
                sizeof(servaddr)) < 0) {
        error("connect error");
        return 1;
    }
        char msg[BUFSIZE];
    while(1)
    {
        printf("Enter msg:");
        scanf("%s" , msg);

        if( send(sockfd , msg , strlen(msg) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         // server reply
        if( recv(sockfd, buf , BUFSIZE , 0) < 0)
        {
            puts("Recv failed");
            break;
        }
        puts("Echo: ");
        puts(buf);
    }
        close (sockfd);
        return 0;

  }
