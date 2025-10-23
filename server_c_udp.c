#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>

int addDigits(char *string)
{
    int sum = 0;

    for (int i = 0; string[i] != '\0'; i++)
    {
        if (isdigit(string[i]))
        {
            int digit = string[i] - '0';
            sum += digit;
        }
        else
        {
            return -1;
        }
    }
    return sum;
}

int main(int argc, char *argv[])
{

    struct sockaddr_in server;
    struct sockaddr_in from;

    char buff[1024];

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }

    // create the UDP socket: AF_INET for IPv4, Datagram socket, 0 for default protocol
    int socketFile = socket(AF_INET, SOCK_DGRAM, 0);
    int length = sizeof(server);

    // clear the structure
    bzero(&server, length);

    // binding the socket to the IP and port number
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    // get the port number from the argument vector and make it an integer
    server.sin_port = htons(atoi(argv[1]));

    // Bind the socket to the server address
    if (bind(socketFile, (struct sockaddr *)&server, length) < 0)
    {
        perror("bind failed");
        exit(0);
    }

    socklen_t fromlen = sizeof(struct sockaddr_in);

    // infinite loop to keep the server running
    while (1)
    {

        // recvfrom returns message, client_address_info
        int message = recvfrom(socketFile, buff, sizeof(buff) - 1, 0, (struct sockaddr *)&from, &fromlen);
        buff[message] = '\0';

        
        for (int i = 0; i < message; i++)
        {
            printf("[%d] = %d ('%c')\n", i, (unsigned char)buff[i], buff[i]);
        }

        if (message < 0)
        {
            perror("recvfrom");
            exit(0);
        }

        // Safely remove trailing newline *only if it's actually there*
        size_t len = strlen(buff);
        if (len > 0 && buff[len - 1] == '\n')
        {
            buff[len - 1] = '\0';
        }

        int sum = addDigits(buff);
        char sum_buffer[1024];
        snprintf(sum_buffer, sizeof(sum_buffer), "%d", sum);

        if (sum == -1)
        {
            const char *error_msg = "Sorry, cannot compute!\n";
            sendto(socketFile, error_msg, strlen(error_msg), 0, (struct sockaddr *)&from, fromlen);
        } else {
            while (1)
            {

                snprintf(sum_buffer, sizeof(sum_buffer), "%d", sum);
                printf("DEBUG: summing '%s'\n", sum_buffer);
                sendto(socketFile, sum_buffer, strlen(sum_buffer), 0,
                       (struct sockaddr *)&from, fromlen);

                if (sum < 10)
                    break;

                sum = addDigits(sum_buffer);
            }
        }
        
    }
    return 0;
}
