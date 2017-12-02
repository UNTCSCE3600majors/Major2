#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>

int sockfd = 0, n = 0;//socket
char recvBuff[1024];//buffer 
struct sockaddr_in serv_addr;
int curr_total = 0;//total count 
char * ip_client; //ip 


void *client_function(void *arg){

    while(1){

        
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);//reads from socket
        
        recvBuff[n] = 0;
            
        printf( "\nValue Received from Server\n");
        int port_number;//port number
            if(strstr(recvBuff,"PORT")!=NULL){
                    sleep(2);
                    printf( "\nConnecting with Other Client\n");
                    char Message_string[1024];//buffer string for messages
                    strcpy(Message_string,recvBuff);//copies message into message string
                    char delim[] = " ";//delimeter assignments 
                    char * pch= strtok(recvBuff,delim);//string conversion
                    pch = strtok(NULL,delim);//port channel assignment

                    port_number = atoi(pch);//ascii to int port number

                    close(sockfd);//closes socket feed

                    int sockfd1 = 0; //new socket open
                    char recvBuff1[1024];//recieves message in this buffer
                    struct sockaddr_in serv_addr1;

                    memset(recvBuff1, '0',sizeof(recvBuff1));//sets 0 in buffer

                    if((sockfd1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)//error checking condition 
                    {
                        printf("\n Error : Could not create socket \n");
                        return 1;
                    } 

                    memset(&serv_addr1, '0', sizeof(serv_addr1)); //sets 0

                    serv_addr1.sin_family = AF_INET;//sets type
                    serv_addr1.sin_port = htons(port_number); //big endian to little endian

                    if(inet_pton(AF_INET, ip_client, &serv_addr1.sin_addr)<=0)//error checking condition
                    {
                        printf("\n inet_pton error occured\n");
                        return 1;
                    } 

                    if( connect(sockfd1, (struct sockaddr *)&serv_addr1, sizeof(serv_addr1)) < 0)//error checking condition
                    {
                       printf("\n Error : Connect Failed \n");
                       return 1;
                    } 

                    char sendBuff[1024];//sending buffer to server

                    sprintf(sendBuff, "%i",curr_total);
                    write(sockfd1, sendBuff, strlen(sendBuff));//sends to server
                    close(sockfd1);//closes socket connection

                    printf("\n Finishing myself. \n");
                    exit(0);



                } else {
                    curr_total = atoi(recvBuff);

                    if (curr_total > 1023 && curr_total < 49152){// threshold condition required in question
                        printf( "\nStarting Own Server");

                        int listenfd = 0, connfd = 0;// listening variables.
                        struct sockaddr_in serv_addr2; 

                        char recvBuff2[1025]; //recieves in this buffer

                        listenfd = socket(AF_INET, SOCK_STREAM, 0);//listen function
                        memset(&serv_addr2, '0', sizeof(serv_addr2));//sets 0 
                        memset(recvBuff2, '0', sizeof(recvBuff2)); //sets 0

                        serv_addr2.sin_family = AF_INET;
                        serv_addr2.sin_addr.s_addr = htonl(INADDR_ANY);
                        serv_addr2.sin_port = htons(curr_total); 

                        bind(listenfd, (struct sockaddr*)&serv_addr2, sizeof(serv_addr2)); //binding function

                        listen(listenfd, 10); //listens

                        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);//accepts connections

                       read(connfd, recvBuff2, sizeof(recvBuff2)-1);//reads from server
                            recvBuff2[n] = 0;
                            
                        printf("\nReceived Value from other Client \n");

                    }



                }
        }
	
}

int main(int argc, char *argv[])// main
{
     

    if(argc != 4) //tells the connected client 
    {
        printf("\n Usage: %s <ip of server> <port> <ip of other client>\n",argv[0]);
        return 1;
    } 
    ip_client = argv[3];
    memset(recvBuff, '0',sizeof(recvBuff));//sets 0
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)//makes connection with other client 
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); //sets 0

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)//error checking 
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)//error checking
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    pthread_t client_thread;
         
    if( pthread_create( &client_thread , NULL ,  client_function , (void*) 1) < 0)//error checking 
    {
        perror("could not create thread");
        return 1; 
    }

    

    while (1){ //sends value from user input in this loop
        
        int val = 0;
        printf( "\nEnter a value :");
        scanf("%i", &val);
        if (val == 0 ){
             close(sockfd);
            exit(0);
            
        } else {
           char sendBuff[1024];
            sprintf(sendBuff, "%i",val);
            int n = write(sockfd, sendBuff, strlen(sendBuff));
           
        }
    }
    pthread_join(client_thread, NULL);// creates thread

     

    

    return 0;
}
