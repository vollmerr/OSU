/***********************************************************************
 *
 *  client.c
 *  Ryan Vollmer - CS 372, Project 1
 *
 *  Simple client side of chat system using threads to allow 
 *      user input and output at the same time.
 *      
 *  Usage: client < ip address > < port number >
 *
 **********************************************************************/

#include "stdio.h" 
#include "stdlib.h"  
#include "stdarg.h"
#include "sys/types.h"  
#include "sys/socket.h"  
#include "string.h"  
#include "netinet/in.h"  
#include "netdb.h"
#include "pthread.h"

#define QUIT "\\quit"
#define BUF_SIZE 490 
#define USER_SIZE 10
#define MSG_SIZE 500

int is_recv = 1;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char recv_buffer[MSG_SIZE];
char send_buffer[MSG_SIZE];


/**
 *  Prints a message in the top left of screen
 */
void print_top(const char* format, ...) {
    va_list args;
    printf("\33[2J\33[;f"); // escape code to clear screen and move currsor to top left
    fflush(stdout);
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}   


int get_recv() {
    int value;
    pthread_mutex_lock(&lock);
    value = is_recv;
    pthread_mutex_unlock(&lock);
    return value;
}

void set_recv(int value) {
    pthread_mutex_lock(&lock);
    is_recv = value;
    pthread_mutex_unlock(&lock);
}


/**
 *  Handles reciving a message from the server
 *  @param {void*} socket   - socket to recieve message from
 */
void* receiveMessage(void* socket) {
    int sockfd, ret;
    sockfd = (int) socket; 

    while (get_recv()) {
        // get message form server
        memset(recv_buffer, 0, MSG_SIZE);  
        ret = recvfrom(sockfd, recv_buffer, MSG_SIZE, 0, NULL, NULL); 
        //  no data or quit server quit
        if (ret < 0 || !strcmp(recv_buffer, QUIT)) {
            set_recv(0);
            break;
        } else if (!get_recv()) {
            break;
        } else {
            // clear input prompt, print message, then reprint prompt
            printf("\33[2K\r"); // escape code for resetting line
            fflush(stdout);    
            printf(recv_buffer);
            printf("\n");
            fflush(stdout);    
            printf(send_buffer);
            fflush(stdout);
        }
    }

    print_top("Disconnected from server. Press Enter to exit.\n");        
    // clean up thread
    pthread_exit(0);
}


/**
*  Initalizes and connects a socket to the server
*  @param {int*} sockfd            - socket file desc to init and connect with
*  @param {sockaddr_in*} addr      - address object for server to initalize
*  @param {char**} argv            - args from cli
*/
void initSocket(int* sockfd, struct sockaddr_in* addr, char**argv) {
    int serverPort;
    char* serverAddr;
    
    serverAddr = argv[1]; 
    serverPort = strtol(argv[2], NULL, 10);
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (*sockfd < 0) {  
        printf("Error creating socket!\n");  
        exit(1);  
    }
    
    memset(addr, 0, sizeof(*addr));  
    addr->sin_family = AF_INET;  
    addr->sin_addr.s_addr = inet_addr(serverAddr);
    addr->sin_port = htons(serverPort);
    
    // connect to server
    print_top("Connecting to \t\t%s:%d\n", serverAddr, serverPort);
    if (connect(*sockfd, (struct sockaddr*) addr, sizeof(*addr)) < 0) {  
        printf("Error connecting to the server!\n");  
        exit(1);  
    }
    print_top("Connected to \t\t%s:%d\n\n", serverAddr, serverPort);
}


/**
*  Handles sending messages to the server for user input
*  @param {int} sockfd            - socket file desc to send messages to
*  @param {sockaddr_in*} addr     - address object of server
*  @param {char*} username        - users name
*/
void sendMessages(int sockfd, struct sockaddr_in* addr, char* username) {
    int len;
    char input_buffer[BUF_SIZE];      
    // prompt for user input with username
    printf("%s> ", username);
    // get input and send while sill reciving from server
    while (get_recv()) {
        // get input from user
        fgets(input_buffer, BUF_SIZE, stdin);
        // remove newline...
        len = strlen(input_buffer) - 1;
        if(len > 0) {
            // remove newline
            if (input_buffer[len] == '\n') input_buffer[len] = 0;
            // exit condition
            if (!strcmp(input_buffer, QUIT)) {
                set_recv(0);
                break;
            }
            // attach username to buffer to send
            snprintf(send_buffer, MSG_SIZE, "%s> %s", username, input_buffer);
            // send the message to server
            if (sendto(sockfd, send_buffer, MSG_SIZE, 0, (struct sockaddr*) addr, sizeof(*addr)) < 0) {
                printf("Error sending data!\n\t-%s", send_buffer);
                break;
            }
            // reset buffer to prompt (for swapText when recieving)
            memset(send_buffer, 0, MSG_SIZE);        
            snprintf(send_buffer, MSG_SIZE, "%s> ", username);
            // reprompt for user input with username
            printf("%s> ", username);
        } 
    }
}


/**
*  Handles getting the user name
*  @param {char*} username     - name to populate
*/
void getUsername(char* username) {
    print_top("Username: ");
    scanf("%s", username);
}


int main(int argc, char**argv) {  
    struct sockaddr_in addr;  
    char username[USER_SIZE];
    int sockfd;  
    pthread_t rThread;
    
    if (argc < 3) {
        printf("usage: client < ip address > < port number >\n");
        exit(1);  
    }
    
    getUsername(username);
    initSocket(&sockfd, &addr, argv);
    
    // create a new thread for receiving messages from the server
    if (pthread_create(&rThread, NULL, receiveMessage, (void*) sockfd)) {
        printf("ERROR: Failed to create thread.");
        exit(1);
    }
    sendMessages(sockfd, &addr, username);

    // cleanup...
    print_top("Disconnected from server.\n");    
    close(sockfd);   
    pthread_cancel(rThread);
    return 0;    
}