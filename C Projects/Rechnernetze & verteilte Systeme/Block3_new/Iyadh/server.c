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

#define ACK_BIT (1<<4)
#define GET_BIT (1<<5)
#define SET_BIT (1<<6)
#define DELETE_BIT (1<<7)

#define BACKLOG 10   // how many pending connections queue will hold

// TODO free stuff
// TODO key void or  char
// does declaring a struct make the compiler allocate memory for it or do I have to allocate it manually
// Element * temp = elementTable[hashKey]; houni for ex
//char void

typedef struct Message // todo change int to a better fitting type
{
    //first Byte
    uint8_t reserved;
    uint8_t ack;
    uint8_t get;
    uint8_t set;
    uint8_t delete;

    //second Byte
    uint8_t keyLengthMSB;

    //third Byte
    uint8_t keyLengthLSB;

    //4th Byte
    uint8_t valLengthB1;

    //5th Byte
    uint8_t valLengthB2;

    //6th Byte
    uint8_t valLengthB3;

    //7th Byte
    uint8_t valLengthB4;

    //Key
    char* Key;

    //Value
    void* Value;

} Message;

// struct Element 
typedef struct Element 
{

    //just return NULL... //bool elementIsFound; // this will be used by the get function to basically tell the execute command function if the element is found or not
    char * Key;
    void * Value;
    void * next;
    long keyLength ; 
    long valueLength;
    short reserved;

}Element;

Element * elementTable[27];



int freeMessage(Message * m){


    free(m->Key);
    free(m->Value);
    free(m)
    ;return 0;
}

int freeElement(Element * e){

    free(e->Key);
    free(e->Value);
    e->next=NULL;
    free(e);
    return 0 ;


}



int getHashKey(char* Key) {

/*   if (Key[0]==NULL)                   // ?? can client give us an empty key ??
    {
        return -1;  

    }else 
*/

    if (65<=Key[0]  && Key[0] <= 90)
    {
        return Key[0]-64;

    }else if (97<=Key[0]  && Key[0] <= 122)
    {
        return Key[0]-96;

    }else {
        return 0;
    }
}


Element * get( char * Key ){
    int hashKey;
    int found = -1;
    if (Key==NULL)
    {
        printf("Key* is NULL\n");
        return NULL;
    } else if (strcmp(Key,"")==0)  //else if (strcmp(Key,"")) :/ false but returns != 0  which means true
    {
        printf("Key* is empty\n");
        return NULL;
    }
    printf("hwew\n");

    hashKey=getHashKey(Key);
    printf("this%d\n", hashKey);
    if (elementTable[hashKey]==NULL)
    {

        printf("nope\n");
        //fflush(stdout); // jeb rabi t3alamtha fel grs
        return NULL;

    }else {
        //printf("why does it come here after setting n deleting\n"); // free doesnt set pointer to null so the if statement was false
        Element * temp = elementTable[hashKey];
        Element * next = temp->next;
        while(next != NULL) {
            if (strcmp(temp->Key,Key)==0 ){
                found=0;
                break;
            }
            temp= next;
            next= next->next;

        }

        if (strcmp(temp->Key,Key)==0 ){
                found=0;
        }

        if (found==0)
        {   
            printf("%d\n",temp->valueLength );
            return temp;
        }else {
            return NULL; }
    }
    
}
    



int insert( Element* e ){ //returns 1 on success -1 on failure //TODO same Key = update
    int hashKey;
    char * Key;
    int found = 1;

    if (e==NULL)
    {
        printf("Element* is NULL\n");
        return -1;
    }

    Key= e->Key;

    hashKey = getHashKey(Key);

    if (elementTable[hashKey]==NULL)
    {
        elementTable[hashKey]=e;
        return 1;
    }else{


        Element * temp = elementTable[hashKey];             // check again for semantic errors too lazy right now
        Element * next = temp->next;
        while(next != NULL) {
            if (strcmp(temp->Key,Key)==0 ){
                found=0;
                break;
            }
            temp= next;
            next = next->next;

        }
        if (strcmp(temp->Key,Key)==0 ){
                found=0;
            }

        if (found==0)
        {   
            free(temp->Value);
            void* newValue = malloc(e->valueLength);
            memcpy(newValue, e->Value,e->valueLength);
            temp->Value= newValue;
            return 1 ;

        } else {

            temp->next=e;
            return 1;
        }
    }                                                                   ////
}


int delete( char* Key ){ //returns 0 on success -1 on failure 
    printf("deletehere\n");
    int hashKey;    
    int found= -1;
    if (Key==NULL)
    {
        printf("Key* is NULL\n");
        return -1;
    }

    hashKey=getHashKey(Key);

    if (elementTable[hashKey]==NULL)
    {
        printf("1\n");
        return -1;

    }else {
        printf("2\n");
        
        Element * prev = elementTable[hashKey];
        Element * temp = elementTable[hashKey];
        Element * next = temp->next;
        while(next != NULL) {
            if (strcmp(temp->Key,Key)==0 ){
                found=0;
                break;
            }
            prev= temp;
            temp= next;
            next= next->next;

        }
        if (strcmp(temp->Key,Key)==0 ){
                found=0;
            }

        if (found==0)
        {
            prev->next = temp->next;

            //prev->next = next;
            printf("deleted\n");
            free(temp);
            //elementTable[hashKey] = NULL;
            temp=NULL;
            return 0 ;
        } else {
            return -1 ;
        }

    }
    
}




Message * getMessageFromReceived(char ** received, long * keyLengthPointer , long * valueLengthPointer ){
    Message * currentMessage = malloc (sizeof(Message));

                                        // should I memset 0 ?
                                        // should I ntoh() ?


    //get reserved,ack,get,set,delete                       // is this the right byte order ?
    uint8_t *firstByte = malloc(1); 
    memcpy(firstByte,*received,1);
    printf("firstByte. = %d\n", *firstByte );
    currentMessage->reserved= *firstByte>>4;         // 1011 0100 --> 0000 1011

    currentMessage->ack= *firstByte & 0b00001000;    // 1011 0100 --> 0000 0000
    currentMessage->ack= currentMessage->ack >> 3;  // 0000 0000 --> 0000 0000

    currentMessage->get= *firstByte & 0b00000100;    // 1011 0100 --> 0000 0100
    currentMessage->get= currentMessage->get >> 2;  // 0000 0000 --> 0000 0001

    currentMessage->set= *firstByte & 0b00000010;    // 1011 0100 --> 0000 0000
    currentMessage->set= currentMessage->set >> 1;  // 0000 0000 --> 0000 0000

    currentMessage->delete= *firstByte & 0b00000001; // 1011 0100 --> 0000 0000

    //get key length
    memcpy(&currentMessage->keyLengthMSB,*received+1,1); 
    memcpy(&currentMessage->keyLengthLSB,*received+2,1);

    //get value length
    memcpy(&currentMessage->valLengthB1,*received+3,1);
    memcpy(&currentMessage->valLengthB2,*received+4,1);
    memcpy(&currentMessage->valLengthB3,*received+5,1);
    memcpy(&currentMessage->valLengthB4,*received+6,1);

    //calculate key Length
    int keyLength = (currentMessage->keyLengthMSB << 8) + currentMessage->keyLengthLSB; // check if this makes sense too lazy right now
    
    printf("set bit = %d\n",currentMessage->set );
    printf("set get = %d\n",currentMessage->get );
    printf("set delete = %d\n",currentMessage->delete );


    //printf("%d\n",keyLength);

    if (keyLength>0){           // I think malloc allocates memory for the content of my pointers so I'm just going to... no it doesn't I have to allocate memory that can contain key and value and  assign the returned malloc pointers to the pointers in my struct.. now my question is if I free my struct pointers will it also free the space I allocated with malloc for key and value? I don't think so?... but the more important question is why the fuck is Timon still reading my uncomprehensive shit?
    
        char * keyBytes = malloc(keyLength);
        memcpy(keyBytes, *received + 7, keyLength);
        currentMessage->Key = keyBytes;
        printf("%d\n", currentMessage->set);
    }else{

                currentMessage->Key = NULL;

    }
    
    long valueLength= currentMessage->valLengthB4 + (currentMessage->valLengthB3 << 8) + (currentMessage->valLengthB2 << 16 ) + (currentMessage->valLengthB1 << 24);
    printf("valueLength = %ld\n", valueLength);

    if (valueLength>0){
        void * valueBytes = malloc(valueLength);
        memcpy(valueBytes, *received + 7 + keyLength, valueLength);
        currentMessage->Value = valueBytes;
    }else{// I had no else statement maybe thats why valgrind is angry about uninictialized values
            // yup thats it
                currentMessage->Value= NULL;

    }

    *keyLengthPointer = keyLength;
    *valueLengthPointer = valueLength;
    return currentMessage; 
}






void * createMessage(Element* element , int acknowlegement, long * totalBytestoSend){ // set 1 delete 0 get 2 

    void * responseMessage;

    uint8_t firstByte;
    if ( acknowlegement == 0)
    {
        firstByte = 0b00001001;
    } else if ( acknowlegement == 1)
    {
        firstByte = 0b00001010;
    } else if ( acknowlegement == 2)
    {
       firstByte = 0b00001100; // this is right im just testing set
           //     firstByte = 0b00001010;

    } else {
        firstByte = 0b00000000;
    }

    if (acknowlegement == 2)
    {
        responseMessage = malloc( 7 + element->valueLength + element->keyLength );  // 
        uint8_t keyLengthMSB = (uint8_t) ( element->keyLength>>8);
        uint8_t keyLengthLSB = (uint8_t) ( element->keyLength & 0xff);
        uint8_t valLengthB1 = (uint8_t) (  element->valueLength >> 24); // & 0xff ) not necessary I think
        uint8_t valLengthB2 = (uint8_t) ( (element->valueLength >> 16) & 0xff );
        uint8_t valLengthB3 = (uint8_t) ( (element->valueLength >> 8) & 0xff );
        uint8_t valLengthB4 = (uint8_t) ( (element->valueLength ) & 0xff );

        memcpy(responseMessage, &firstByte , 1);
        memcpy(responseMessage+1, &keyLengthMSB , 1);
        memcpy(responseMessage+2, &keyLengthLSB , 1);
        memcpy(responseMessage+3, &valLengthB1 , 1);
        memcpy(responseMessage+4, &valLengthB2 , 1);
        memcpy(responseMessage+5, &valLengthB3 , 1);
        memcpy(responseMessage+6, &valLengthB4 , 1);

        memcpy(responseMessage+7, element->Key, element->keyLength);
        memcpy(responseMessage+7+ element->keyLength, element-> Value, element->valueLength);

        *totalBytestoSend = 7 + element->keyLength + element->valueLength;
        //printf("%s\n",element->Key );
        return responseMessage;

    } else
    {
        responseMessage = malloc( 7 );
        uint8_t zero = 0 ;
        memcpy(responseMessage, &firstByte , 1);
        memcpy(responseMessage+1, &zero , 1);
        memcpy(responseMessage+2, &zero , 1);
        memcpy(responseMessage+3, &zero , 1);
        memcpy(responseMessage+4, &zero , 1);
        memcpy(responseMessage+5, &zero , 1);
        memcpy(responseMessage+6, &zero , 1);

        *totalBytestoSend = 7 ; return responseMessage;

    }

}



Element * createElement(Message * currentMessage, long keyLength, long valueLength ){

    Element * currentElement = malloc(sizeof(Element));


    // allocate memory again because we're freeing currentMessage
    char * keyBytes = malloc(keyLength);
    void * valueBytes = malloc(valueLength);

    memcpy(keyBytes,currentMessage->Key,keyLength);
    memcpy(valueBytes,currentMessage->Value,valueLength);

    currentElement->Key = keyBytes;
    currentElement->Value = valueBytes;
    currentElement->keyLength = keyLength;
    currentElement->valueLength = valueLength;

    //fwrite("createElementFunctionsKey: %s\n", currentElement->Key);
    


    currentElement->next=NULL; // otherwise valgrind will be angry
    short reserved=0;



    return currentElement;
}


void * executeCommand(Message * currentMessage, long * keyLengthPointer , long * valueLengthPointer, long * totalBytestoSend){
    //Message * responseMessage = malloc(sizeof(Message));


    if (currentMessage->set == 1){
        Element* currentElement = createElement(currentMessage, *keyLengthPointer , *valueLengthPointer);
        return createMessage(NULL,insert(currentElement),totalBytestoSend) ; // if insert returns -1 return falure message if it returns 1  return ack message

    }
 
    if (currentMessage->get == 1){ //-1 2
        //printf("%s::\n", currentMessage->Key);
        Element* currentElement = get(currentMessage->Key);
        if (currentElement==NULL)
        {

            return createMessage(currentElement,-1,totalBytestoSend);
        }
        return createMessage(currentElement,2,totalBytestoSend);

       
    }

    if (currentMessage->delete == 1){// -1 0
       return createMessage(NULL,delete(currentMessage->Key),totalBytestoSend);
    }

}




void garbage_collector(){
    for(int i=0; i<28;i++){
        if(elementTable[i] != NULL){
            Element *tmp = elementTable[i];
            Element *elemDelete;
            elementTable[i] = NULL;
            while(tmp->next != NULL){
                elemDelete = tmp;
                tmp = tmp->next;
                free(elemDelete->Value);
                free(elemDelete->Key);
                free(elemDelete);
            }
            free(tmp->Value);
            free(tmp->Key);
            free(tmp);
        }
    }
    exit(1);
}


int main(int argc, char *argv[])
{

    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    signal(SIGINT,garbage_collector);
    //received = malloc(1000);


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP


    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;

    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop
        
        
        /// Accept Connection
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size); if (new_fd == -1) {perror("accept");continue;}
        printf("accepted\n");

        int numbytes = 0;
        int totalBytesReceived=0;    // iyadh stop being stupid if you dont reset to 0 u will keep incrementing this variable forever lol stoopit
        char* received = malloc(3000000);
        char* buf = malloc(512);  //Q?: if it's not void how does it treat it as a simple byte
        long totalBytestoSend=0;

        long  keyLength =0;
        long  valueLength =0 ;
        
        memset( received, '\0', sizeof(char)*3000000 );
        memset( buf, '\0', sizeof(char)*512 );

        /// Receive Client Request
        while(1){
        
            if ((numbytes = recv(new_fd, buf, 512, 0)) == -1) { //new_fd or sockfd?
                perror("recv");
                exit(1);
            }

            buf[numbytes] = '\0';
            printf("%d\n", numbytes);

            memcpy(received+totalBytesReceived,buf,numbytes);
            totalBytesReceived+=numbytes;
            if (numbytes<512) {printf("break\n"); break;} // ==0
        }


        

       // uint8_t *firstByte = malloc(1);  memcpy(firstByte,received,1);printf("firstByte. = %d\n", *firstByte );

        Message * currentMessage = getMessageFromReceived(&received, &keyLength, &valueLength); // this will save the contents of the message recieved in currentMessage
        //printf("%d\n", keyLength );

        


        void * responseAsBytes = executeCommand(currentMessage, &keyLength, &valueLength, &totalBytestoSend);
        

        //totalBytestoSend = 7 + keyLength + valueLength; // keyLength is 0 for set and delete.........


        int bytesSent = 0;
        bytesSent = send(new_fd, responseAsBytes, totalBytestoSend , 0); 


        free(received);
        freeMessage(currentMessage);// also free the pointers you created   
        

        close(new_fd);
        
    }
    

    return 0;
}