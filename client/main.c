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
#include<errno.h> //For errno - the error number

#define BUFSIZE 1024

int hostname_to_ip(char *  , char *);

void error(char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc , char *argv[]) {
    int sockfd, portno, n;
    char buf[BUFSIZE], msg[BUFSIZE];

    if(argc != 3) {
      fprintf(stderr, "Aloita kirjoittamalla: %s <hostname> <portnumber>\n", argv[0]);
      return -1;

    }
    // IP
    char *hostname = argv[1];
    char ip[100];

    //Porttinumero
    portno = atoi(argv[2]);


    hostname_to_ip(hostname , ip);
    printf("%s resolved to %s\n" , hostname , ip);


    // define variables


    struct sockaddr_in servaddr; // tietorakenne, joka esittää osoitetta

    // Nämä myöhemmin DNS-kyselyllä

    struct hostent *gethostbyname(const char *address);
    const char *address = ip;

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
    servaddr.sin_port   = htons(portno);


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
    puts("Connected\n");


// Viestien lähetys
    // Tähän for loop viesteille/vastauksille
    while(1)
    {
    bzero(buf, BUFSIZE);
    fgets(buf, BUFSIZE, stdin);

    n = read(sockfd, buf, BUFSIZE);
    if (n < 0)
      error("ERROR reading from socket");
    printf("%s\n", buf);

    printf("Write message: ");
    bzero(msg, BUFSIZE);
    fgets(msg, BUFSIZE, stdin);

    /* send the message line to the server */
    n = write(sockfd, msg, strlen(msg));
    if (n < 0)
      error("ERROR writing to socket");


    /* print the server's reply */

    }

    close(sockfd);
    return 0;


    /*
    int i = 0;

    while (i != 1) {
    const char *tosend = "Jee toimii\n";
    size_t paluuarvo;
    paluuarvo = write(sockfd, tosend, strlen(tosend));
    printf("ho"\n", );
    if(paluuarvo < 0) {
      perror("writing error");
      return 1;
    }

    char vastaus[160];
    memset(vastaus, 0, 160); // täytetään puskuri nollilla
    // Lue palvelimen vastaus yllä määriteltyyn puskuriin, jossa on tilaa
    // 160 merkille.
    read(sockfd, vastaus, 160);

    vastaus[159] = 0;  // varmistetaan että lopussa on nolla
    printf("%s\n", vastaus);  // tulostetaan luettu teksti ruudulle
    scanf("Give i: %i\n", &i );


  }
  */
}


// IP HAKEMINEN

int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }

    return 1;
}
