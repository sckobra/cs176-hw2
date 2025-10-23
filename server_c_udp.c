#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    struct sockaddr_in server;
    struct sockaddr_in from;

    char buff[1024];
    
    if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }


    //create the UDP socket: AF_INET for IPv4, Datagram socket, 0 for default protocol
    int socketFile = socket(AF_INET, SOCK_DGRAM, 0);
    int length = sizeof(server);

    //clear the structure
    bzero(&server,length);

    //binding the socket to the IP and port number
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    
    //get the port number from the argument vector and make it an integer
    server.sin_port = htons(atoi(argv[1]));

    // Bind the socket to the server address
    if (bind(socketFile, (struct sockaddr *)&server, length) < 0) {
        perror("bind failed");
        exit(0);
    }

    int fromlen = sizeof(struct sockaddr_in);

    //infinite loop to keep it running
    while(1) {
        //recvfrom returns message, client_address_info
        int message = recvfrom(socketFile, buff, 1024, 0, (struct sockaddr *)&from,&fromlen);

        if (message < 0){
            perror("recvfrom");
            exit(0);
        }

        write(1, "Received a datagram: ",21);
        write(1, buff, message);
        message = sendto(socketFile,"Got your message\n",17, 0, (struct sockaddr *)&from, fromlen);
        if (message < 0){
            perror("sendto");
            exit(0);
        } 
    }
    return 0;
    



}