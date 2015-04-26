#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>


using namespace std;

typedef enum{AUTO, MANUEL}Process;
typedef enum{Avance, Recule, Gauche, Droite}Nav;
typedef enum{String = 1, Valeur, Position_servo}Requette;


typedef struct {
    char size;
    char msg_type;
}tcp_header;

typedef struct {
    int valeur;
    int servo;
}tcp_changement_pos;

typedef struct{
    int Pos_act;
    int Pos_com;
}Servo_t;

typedef struct{
    int Socket;
    //std::array<Servo_t,4> Position_servo;
    Servo_t Position_servo[4];
    Process process;
    Nav navigation;
}Commande;

Commande PI;

void error(const char *msg)
{
    perror(msg);
    //exit(1);
}

void SendTcp(int FD, char msg_type, char msg_size, void* Payload)
{
    int n;
    tcp_header client_header;

    client_header.msg_type = msg_type;
    client_header.size = msg_size;

    n = send(FD,&client_header,sizeof(client_header),0);
    if (n < 0) error("ERROR writing to socket");

    n = send(FD,Payload,client_header.size,0);
    if (n < 0) error("ERROR writing to socket");
}


void* Maitre(void* arg)
{
    return NULL;
}

void* PosServo(void* arg)
{
    int sendposition[4];
    int i;
    bool SendFlag = false;
    /*
    sendposition[0] = 3;
    sendposition[1] = 56;
    sendposition[2] = 34;
    sendposition[3] = 98;

    SendTcp(PI.Socket,2,4*sizeof(int), sendposition);
    */

    while(1)
    {
        for(i=0;i<4;i++)
            PI.Position_servo[i].Pos_act = PI.Position_servo[i].Pos_com;

        for(i=0;i<4;i++)
        {
            if(sendposition[i] != PI.Position_servo[i].Pos_act){
                sendposition[i] = PI.Position_servo[i].Pos_act;
                SendFlag = true;
            }
        }

        if ( SendFlag == true)
        {
            SendTcp(PI.Socket,2,4*sizeof(int), sendposition);
            SendFlag = false;
        }

        usleep(10000);
    }
    return NULL;
}

void* Client(void* arg){

    int n, sockfd, exit_code = 0, test;

    tcp_header client_header;
    tcp_changement_pos Chang_position;

    sockfd = *((int*)arg);
    char buffer[255];
    string messages;
    bzero(buffer,256);

    while(exit_code != 1)
    {
        //Reception des donnée header (Taille et type du message)
        n = recv(sockfd, &client_header, sizeof(client_header),0);
        if (n < 0) {
            perror("ERROR recive socket");
            exit_code = 1;
            return NULL;
        }


        test = (int)client_header.size;
        cout << "Nb byte lu : " << n << endl;
        cout << "Msg size : " << test << " type : " << (int)client_header.msg_type << endl;

        //Reception du message
        if (client_header.size != 0){
           n = recv(sockfd, buffer, test,0);
        }
        if (n < 0)
        {
                perror("ERROR recive socket");
                exit_code = 1;
                return NULL;
        }
        else
        {
            cout << "Nb byte lu : " << n << endl;
            if (client_header.msg_type == String){
                messages.clear();
                messages.append(buffer,test);
                cout << messages << endl;
                SendTcp(sockfd,1,messages.length(),(void*)messages.c_str());
            }
            if (client_header.msg_type == Valeur){
                int valeur = (int)buffer[0];
                cout << valeur << endl;
            }

            /*Message recu lors d'une commande manuel pour changer la position des servos*/
            if (client_header.msg_type == 3){

                Chang_position = *(tcp_changement_pos*)buffer;

                cout << "Servo ID :" << Chang_position.servo << endl;
                cout << "Value    :" << Chang_position.valeur << endl;

                PI.Position_servo[Chang_position.servo - 1].Pos_com = Chang_position.valeur;
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     pthread_t t1, ServoT;

     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

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

         cout << "New connection" << endl;

         PI.Socket = newsockfd;

         //bzero(buffer,256);

         pthread_create(&t1, NULL, Client, (void*) &newsockfd) ;
         pthread_create(&ServoT, NULL, PosServo, (void*) NULL);
         pthread_join(t1,0);
         close(newsockfd);
     }

     close(sockfd);
     return 0;
}
