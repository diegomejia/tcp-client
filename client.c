/*
    -- Author: Diego Mejia
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
#define PORT 11111 
#define ERROR -1

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
        while ( stringBuffer[n] != '\n' && stringBuffer[n] != '\r' && n < MAX);

        stringBuffer[n] = '\0';

        write(socketFD, stringBuffer, sizeof(char) * n); 
        bzero(          stringBuffer, sizeof(stringBuffer)); 
        read (socketFD, stringBuffer, sizeof(char) * n); 
        
        printf("from server: %s", stringBuffer); 
        
        if((strncmp(stringBuffer, "exit", 4)) == 0) 
        {
            printf("tcp-client is shutting down...\n");
            break; 
        } 
    }
} 
  
int main() 
{ 
    int socketFD; 
    struct sockaddr_in serverAddress; 
  
    // Socket File Descriptor creation and verification.
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    if(socketFD == ERROR) { 
        printf("Socket creation failed...\n"); 
        exit(0); 
    } 
    else {
        printf("Socket successfully created.\r\n"); 
    }

    bzero( &serverAddress, sizeof(serverAddress) ); 
  
    // Assign IP, PORT 
    serverAddress.sin_family      = AF_INET; 
    serverAddress.sin_addr.s_addr = inet_addr("82.182.82.182"); 
    serverAddress.sin_port        = htons(PORT);
  
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
