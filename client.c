/*
    -- Author: Diego Mejia
    If you remove authorship you're a prick. 
    RedWall Studios LLC ©
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>

#define MAX 15
#define DEFAULT_IPV4_ADDR "82.182.82.182" // Default values.
#define DEFAULT_PORT 11111

#define ERROR -1
#define NO_ARGS 1
#define VALID_NUM_ARGS 5

#define IPV4_ARG "-ipv4"
#define PORT_ARG "-port"

void infinite(int socketFD)
{
    char stringBuffer[MAX+1]; // Leave space for null.
    int  n = 0;
    for(;;)
    {

        bzero(stringBuffer, sizeof(stringBuffer));
        printf("enter a string: ");

        do
        {
            stringBuffer[n] = getchar();
            n++;
        }
        while ( stringBuffer[n-1] != '\n' && stringBuffer[n-1] != '\r' && n <= MAX);

        stringBuffer[n] = '\0';

        write(socketFD, stringBuffer, sizeof(char) * n);
        bzero(          stringBuffer, sizeof(stringBuffer));
        read (socketFD, stringBuffer, sizeof(char) * n);

        n = 0; // Reset n, for follow-up input string.

        printf("from server: %s\r\n", stringBuffer);

        if((strncmp(stringBuffer, "exit", 4)) == 0)
        {
            printf("tcp-client is shutting down...\r\n");
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int socketFD;
    struct sockaddr_in serverAddress;

    // Socket File Descriptor creation and verification.
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD == ERROR) {
        printf("Socket creation failed...\r\n");
        exit(0);
    }
    else {
        printf("Socket successfully created.\r\n");
    }

    bzero( &serverAddress, sizeof(serverAddress) );

    // Assign IP, PORT
    serverAddress.sin_family      = AF_INET;

    // Check for use of command line arguments.
    if(argc > NO_ARGS && argc == VALID_NUM_ARGS){
      if((strncmp(argv[1], IPV4_ARG, 5)) == 0 &&
         (strncmp(argv[3], PORT_ARG, 5)){
                serverAddress.sin_addr.s_addr = inet_addr(argv[2]);
                serverAddress.sin_port = htons((unsigned short)atoi(argv[4]));
      }
      else if((strncmp(argv[3], IPV4_ARG, 5)) == 0 &&
              (strncmp(argv[1], PORT_ARG, 5)) == 0    ){
                serverAddress.sin_addr.s_addr = inet_addr(argv[4]);
                serverAddress.sin_port = htons((unsigned short)atoi(argv[2]));
      }
    } else {
      serverAddress.sin_addr.s_addr = inet_addr(DEFAULT_IPV4_ADDR);
      serverAddress.sin_port        = htons(DEFAULT_PORT);
    }

    // Connect the client socket to server socket.
    if (connect(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) != 0) {
        printf("Connection attempt failed...\r\n");
        exit(0);
    }
    else{
        printf("Connected to the server!\r\n");
    }

    // Send the data, read the data
    infinite(socketFD);

    // close the socket
    close(socketFD);
}
