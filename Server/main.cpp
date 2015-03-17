/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <pthread.h>


pthread_mutex_t suspend = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void* Client(void* arg){

    int n, sockfd, exit_code = 0;
    sockfd = *((int*)arg);
    char buffer[255];
    string message;
    n = write(sockfd,"Ready !",7);
    if (n < 0) error("ERROR writing to socket");
    cout << "New connection" << endl;
    while(exit_code != 1)
    {
        n = recv(sockfd, buffer,255,0);
        if (n < 0) {
            perror("ERROR recive socket");
            exit_code = 1;
        }
        else{
            message = buffer;
            cout << message << endl;

            n = write(sockfd,"Message recu !",14);
            if (n < 0) error("ERROR writing to socket");
        }
    }
    //pthread_mutex_unlock(&suspend);
    return NULL;
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     pthread_t t1;
     socklen_t clilen;
     char buffer[255];
     struct sockaddr_in serv_addr, cli_addr;

     pthread_mutex_lock(&suspend);

     //int n;
     if (argc < 2) {
         cout << stderr << " ERROR, no port provided" << endl;
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     //bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     cout << "Le serveur demarre sur le port " << portno << endl;

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

     while(1)
     {
         listen(sockfd,1);
         clilen = sizeof(cli_addr);
         newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);

         if (newsockfd < 0)
              error("ERROR on accept");

         bzero(buffer,256);

         pthread_create(&t1, NULL, Client, (void*) &newsockfd) ;
         /*n = read(newsockfd,buffer,255);
         if (n < 0) error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);*/
         pthread_join(t1,0);
         //pthread_mutex_lock(&suspend);
         close(newsockfd);
     }

     close(sockfd);
     return 0;
}
