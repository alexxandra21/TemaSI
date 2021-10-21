#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include<unistd.h>


#define MAX 80
#define PORT 8080
#define SA struct sockaddr
   
// Driver function
int main()
{
    int sockfd1, connfd1,sockfd2, connfd2, len;//sockfd1 pt A-B , sockfd2 pt A-KM
    struct sockaddr_in servaddr1, cli1;
    struct sockaddr_in servaddr2, cli2;
    unsigned char iv[129]={"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"};
    AES_KEY * key;

    // socket1 create and verification
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr1, sizeof(servaddr1));
   
    // assign IP, PORT
    servaddr1.sin_family = AF_INET;
    servaddr1.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr1.sin_port = htons(PORT);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd1, (SA*)&servaddr1, sizeof(servaddr1))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen and verification
    if ((listen(sockfd1, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli1);
   
    // Accept the data packet from client and verification
    connfd1 = accept(sockfd1, (SA*)&cli1, &len);
    if (connfd1 < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
//socket 2 create and verification
 sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd2 == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr2, sizeof(servaddr2));

    // assign IP, PORT
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr2.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd2, (SA*)&servaddr2, sizeof(servaddr2))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd2, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli2);

    // Accept the data packet from client and verification
    connfd2 = accept(sockfd2, (SA*)&cli2, &len);
    if (connfd2 < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");

   printf("Pentru a alege modul de comunicare intre a si B va rugam introduceti 1 pentru ECB sau 2 pentru CBC.");
   int mode=0;
   while(1)
   { scanf("%d",&mode);
	   if (mode==1)
	   { //send ECB
            write(sockfd1, "ECB", sizeof(3));
           break;
	   }
	   else
	   { //send CBC
 	    write(sockfd1, "CCB", sizeof(3));
	   break;
	   }
    printf("Va rugam introduceti 1 sau 2");
  }
   char K[MAX];
   char msg[MAX];
   write(sockfd2,"Send key K ", sizeof(11));//Cer K din KM
   read(sockfd2,K, sizeof(K));//citesc K din KM
  write(sockfd1,K, sizeof(K));//trimit K lui B
  //decriptez K
   AES_decrypt(K,K,key);
  read(sockfd1,msg, sizeof(msg));//primeste mesaj de confirmare de la B
  char input_filename[] = "mesajA.txt";
    // deschidere fisier de intrare pentru
    // citire (r) in modul text (t)
    FILE *in = fopen(input_filename, "rt");

    // verific daca fisierul a fost deschis cu succes
    // altfel opresc executia (in cazul acestei probleme)
    if (in == NULL) {
        fprintf(stderr, "ERROR: Can't open file %s", input_filename);
        return -1;}
   fseek(in, 0, SEEK_SET);
   char kprim[129];//kprim
   char block[129];
   while(!EOF)
   {
	if (fread(block,128,1, in) != 1) abort();
	 memset(&msg[strlen(msg)], 0, 128 - strlen(msg));//padding with 0
	
	 //cripteaza blockrile
	 //
	 //trimite lui B blocuri criptate
	 write(sockfd1,block,128);
   }
	write (sockfd1,"exit",sizeof("exit"));

    // After chatting close the socket
    close(sockfd1);
    close(sockfd2);
}
