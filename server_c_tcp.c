#include <stdio.h> //from linux how to
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int sum_digits(const char *str) { // sum digits in a string
    int sum = 0;
    for (int i = 0; str[i]; i++) {
        if (isdigit(str[i]))
            sum += str[i] - '0';
    }
    return sum;
}

int is_numeric(const char *str) { // check if string is numeric
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[129];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) { 
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        memset(buffer, 0, sizeof(buffer));
        
        n = read(newsockfd, buffer, 128);
        if (n < 0) error("ERROR reading from socket");

        buffer[n] = '\0';

        
        if (!is_numeric(buffer)) {
            write(newsockfd, "Sorry, cannot compute!", 22);
            close(newsockfd);
            continue;
        }

        // repeatedly sum digits until one digit remains
        int value = sum_digits(buffer);
        char result[32];

        while (value >= 10) {
            snprintf(result, sizeof(result), "%d", value);
            write(newsockfd, result, strlen(result));
            sleep(0.1); 
            value = sum_digits(result);
        }

        snprintf(result, sizeof(result), "%d", value);
        write(newsockfd, result, strlen(result));

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
