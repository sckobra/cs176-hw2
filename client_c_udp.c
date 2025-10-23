#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    struct sockaddr_in server;


    // create the UDP socket: AF_INET for IPv4, Datagram socket, 0 for default protocol
    int socketFile = socket(AF_INET, SOCK_DGRAM, 0);
    int length = sizeof(server);

    // clear the structure
    bzero(&server, length);

    // binding the socket to the IP and port number
    server.sin_family = AF_INET;

    // get the  IP from command line arg
    server.sin_addr.s_addr = inet_addr(argv[1]);

    // get the port number from the argument vector and make it an integer
    server.sin_port = htons(atoi(argv[2]));

    char user_input[128];

    // user input
    printf("Enter string: ");
    fgets(user_input, sizeof(user_input), stdin);

    sendto(socketFile, user_input, strlen(user_input), 0, (const struct sockaddr *)&server, length);

    // server_response
    char res[255];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);

    while (1)
    {
        int message = recvfrom(socketFile, res, sizeof(res), 0, (struct sockaddr *)&from, &fromlen);

        if (message < 0)
        {
            perror("Error receiving from server");
            break;
        }

        // add null terminator at the end of the received bytes
        res[message] = '\0';

        // print the message from the server
        printf("From server: %s", res);

        // check for end conditions
        if (strcmp(res, "Sorry, cannot compute!\n") == 0)
        {
            break;
        }

        // check if the message is a single character (final digit)
        if (strlen(res) == 1)
        {
            break; // server finished computing
        }
    }
    close(socketFile);
    return 0;
}