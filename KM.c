#include <netdb.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <arpa/inet.h>


#define MAX 80
#define PORT 8080
#define SA struct sockaddr
//KM
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    unsigned char iv[129]={"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"};   
    AES_KEY *key;//initialize key k'
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
   
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
	unsigned char k[129]; 
 	//genereaza k random
    memset(k, 0, 16);
    if (!RAND_bytes(k, 16))
        exit(-1);	
	//encrypt k cu aes folosind k'
	AES_encrypt(k, k, key);
	//trimit k la A
 	write(sockfd, k,128);   
   
    // close the socket
    close(sockfd);
}
