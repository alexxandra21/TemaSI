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
#include<arpa/inet.h>



#define MAX 80
#define PORT 8080
#define SA struct sockaddr
//B
int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    unsigned char iv[129]={"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"};   
    AES_KEY *key;
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
   // bzero(&servaddr, sizeof(servaddr));
   memset(&servaddr,0,sizeof(servaddr));
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
   char kprim[129];//kprim 
    char k[129];
    char mode[4];
    read(sockfd,mode,sizeof(mode));
    printf("%s",mode);
    read(sockfd,k, sizeof(k)); //primeste K de la A
   printf("%s",k);
    //decripteaza k
   AES_decrypt(k,k,key);

   
    write(sockfd,"B is ready.",sizeof("B is ready.")); //Trimite lui A mesaj de confirmare
    char block[129];
    char input_filename[] = "mesajB.txt";
    // deschidere fisier de intrare pentru
    FILE *out = fopen(input_filename, "wt");
	if (out == NULL) {
        fprintf(stderr, "ERROR: Can't open file %s", input_filename);
        return -1;}
   fseek(out, 0, SEEK_SET);


    read(sockfd, block,128);
    while(strncmp(block,"exit",4)!=0)
    {
	    if(strncmp(mode,"ECB",3)==0)
	    	AES_encrypt(block,block,key);
		    //decript block cu ecb
	    
	    if(strncmp(mode,"CBC",3)==0)
	   	AES_cbc_encrypt(block,block,128,key,iv,1);
		    //deript block cu cbc


	    //scrie block in fisier
	    fwrite(block,128,1,out);
    }

    // close the socket
    close(sockfd);
}
