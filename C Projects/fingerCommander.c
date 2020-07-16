#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include <math.h>

#define BACKLOG 200   // how many pending connections queue will hold

typedef struct InternalMessage{

	uint8_t control;

	uint8_t finger;
	uint8_t fAck;

	uint8_t join;
	uint8_t notify;
	uint8_t stabilize;

	uint8_t reply;
	uint8_t lookUp;

	uint16_t hashID;
	uint16_t nodeID;

	char * IP;

	uint32_t nodeIP;

	uint8_t IP1;
	uint8_t IP2;
	uint8_t IP3;
	uint8_t IP4;

	uint16_t nodePort;

} InternalMessage;

InternalMessage * fillFT(){

    InternalMessage * internalMessage = calloc(1,sizeof(InternalMessage));

    internalMessage->control= 1;

    internalMessage->finger = 1;
    internalMessage->fAck   = 0;
    internalMessage->join   = 0;
    internalMessage->notify = 0;
    internalMessage->stabilize = 0;

    internalMessage->reply  = 0;
    internalMessage->lookUp = 0;
    internalMessage->hashID = 0;
    internalMessage->nodeID = 0;

    internalMessage->nodeIP = 0;


    internalMessage->IP1 = 0;
    internalMessage->IP2 = 0;
    internalMessage->IP3 = 0;
    internalMessage->IP4 = 0;

    internalMessage->nodePort = 0;

    internalMessage->IP = NULL;

    return internalMessage;

}


void* serializeInternalMessage(InternalMessage * iM){
        void* serialized = calloc(11,1);

        uint8_t firstByte;
        firstByte = iM->control << 7 & 0b10000000;

        firstByte = firstByte | (iM->finger << 6 & 0b01000000);
        firstByte = firstByte | (iM->fAck << 5 & 0b00100000);
        firstByte = firstByte | (iM->join << 4 & 0b00010000);
        firstByte = firstByte | (iM->notify << 3 & 0b00001000);
        firstByte = firstByte | (iM->stabilize << 2 & 0b00000100);

        firstByte = firstByte | (iM->reply << 1 & 0b00000010 );
        firstByte = firstByte | (iM->lookUp & 0b00000001);

        memcpy(serialized,&firstByte,1);

        uint8_t hashIDMSB = (iM->hashID >> 8) & 0xff;
        uint8_t hashIDLSB = (iM->hashID & 0xff) ;

        memcpy(serialized+1, &hashIDMSB , 1);
        memcpy(serialized+2, &hashIDLSB , 1);

        uint8_t nodeIDMSB = (iM->nodeID >> 8) & 0xff;
        uint8_t nodeIDLSB = (iM->nodeID & 0xff) ;

        memcpy(serialized+3, &nodeIDMSB , 1);
        memcpy(serialized+4, &nodeIDLSB , 1);

        uint8_t nodeIPB1 = (iM->IP1);
        uint8_t nodeIPB2 = (iM->IP2);
        uint8_t nodeIPB3 = (iM->IP3);
        uint8_t nodeIPB4 = (iM->IP4); 

        memcpy(serialized+5, &nodeIPB1 , 1);
        memcpy(serialized+6, &nodeIPB2 , 1);
        memcpy(serialized+7, &nodeIPB3 , 1);
        memcpy(serialized+8, &nodeIPB4 , 1);

        uint8_t nodePortMSB = (iM->nodePort >> 8) & 0xff;
        uint8_t nodePortLSB = (iM->nodePort & 0xff) ;

        memcpy(serialized+9, &nodePortMSB , 1);
        memcpy(serialized+10, &nodePortLSB , 1);

        return serialized;
}



int main(int argc, char *argv[])
{
    char* received = malloc(10000) ;  // full text received from server

    int sockfd, numbytes;  
    char buf[512];
    struct addrinfo hints, *servinfo, *p;
    int rv;

    char* host ;    // server's domain name or ip
    char* port ;    // server's port nummber

    if (argc != 3) {
        fprintf(stderr,"usage: ./fCommander hostname port\n");
        exit(1);
    }

    host = argv[1];
    port = argv[2];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(servinfo); // all done with this structure

    InternalMessage * iM = fillFT();
    void* serialized = serializeInternalMessage(iM);
    send(sockfd, serialized, 11 , 0);
    

    int totalBytesReceived=0;
  
    while(1){
        
        if ((numbytes = recv(sockfd, buf, 512, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        //printf("%d\n",numbytes );

        buf[numbytes] = '\0'; // is this allowed ?

        memcpy(received+totalBytesReceived,buf,numbytes);
        totalBytesReceived+=numbytes;
        if (numbytes==0) break;
    }

    //buf[numbytes] = '\0';
    long * a = calloc(sizeof(long),1);
    memcpy(a,received,1);
    printf("%ld\n", *a );
    //fwrite(received , 1 , 1, stdout );
    //printf("\n");
    //printf("%d\n", totalBytesReceived );
    free(received);

    close(sockfd);



	return 0;
}