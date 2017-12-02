#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int listenfd = 0, connfd = 0, connfd1 = 0, connfd2 = 0;//declarations of required variables
struct sockaddr_in serv_addr;
int client1 = 0;
int client2 = 0;
int t_clients = 0;

void *server_function(void *arg){ //server function in which server performs required functionality 

    int number = (int) arg;

    int sockfd = 0;
    int sockfd2 = 0;

    int total = 0;


    printf("Socket thread %i\n", sockfd);
    printf("Socket 2 thread %i\n", sockfd2);
    char recvBuff[1024];//receiving buffer
    while(1){//connects to client
        int n = 0 ;
        printf("\n Waiting for Client%i Value \n", number); 
        if (number == 1){ //read from first client
                n = read(connfd1, recvBuff, sizeof(recvBuff)-1);
                recvBuff[n] = 0;
            } else {
                n = read(connfd2, recvBuff, sizeof(recvBuff)-1);// reads from 2nd client
                recvBuff[n] = 0;
            }
                
   
    
        if(n <= 0)//clients connected or not? checking
        {
            printf("\n Client Disconnected \n");

            if (number == 1){//one client connected
                client1 = 0;
                t_clients = t_clients - 1;
            } else {// 2nd client connected
                 client2 = 0;
                t_clients = t_clients - 1;
            }
            break;

        } 
        printf("\n Value Received from Client%i \n", number);
        total = total + atoi(recvBuff);//recieves number from clients 

        
        if (total > 1023 && total < 49152){//threshold checking 

            if (t_clients == 2){// checks if 2 clients are connected or not
                char sendBuff2[1024];//sending buffer
                printf("\n Value Within Thresh Sending to other client \n");
                bzero(sendBuff2,1024);//bits changing into 0
                sprintf(sendBuff2, "PORT %i",total);// sends total of client as a port number to other client
                if (number == 1){
                     write(connfd2, sendBuff2, strlen(sendBuff2));//writes to client 2
                } else {
                     write(connfd1, sendBuff2, strlen(sendBuff2));//writes to client 1
                }
               
            } else {//resets the values 
                printf("\n Value Within Thresh Resetting Value \n");
                total = 0;
            }

        }

        char sendBuff[1024];// sending buffer

        sprintf(sendBuff, "%i",total);//sends the total 

        if (number == 1){
             write(connfd1, sendBuff, strlen(sendBuff));//writes to client 1
        } else {
             write(connfd2, sendBuff, strlen(sendBuff));// writes to client 2
        }
    }

}

int main(int argc, char *argv[])
{

    if(argc != 2) // checks which client is using server right now
    {
        printf("\n Usage: %s <port>\n",argv[0]);
        return 1;
    } 
     

    char sendBuff[1025];//sending buffer
    time_t ticks; //timer ticks

    listenfd = socket(AF_INET, SOCK_STREAM, 0);//listens from socket
    memset(&serv_addr, '0', sizeof(serv_addr));//sets 0
    memset(sendBuff, '0', sizeof(sendBuff)); //sets 0
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1])); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); //binds

    listen(listenfd, 10); //listening function
pthread_t server_thread;// threading 
    while(1)//loop for connections. 
    {
	   printf("\n connect\n");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); //accepts connection from socket
        printf("\n connected\n");
        
        int curr_cli = 0;//number of clients

        if (client1 == 0){ //if 1 client is connected 
            curr_cli = 1;
            client1 = 1;
            t_clients += 1;
            connfd1 = connfd; 
        } else {//if 2 clients are connected 
             curr_cli = 2;
            client2 = 1;
            t_clients += 1;
            connfd2 = connfd;
        }
         printf("number = %i\n", curr_cli);
         printf("Socket %i\n", connfd);
        if( pthread_create( &server_thread , NULL ,  server_function , (void*) curr_cli) < 0)//threading error checking
        {
            perror("could not create thread");
            return 1; 
        }

        
        printf( "Activating Client \n");

        while (t_clients == 2){

        }

        
     }
     pthread_join(server_thread, NULL);
}
